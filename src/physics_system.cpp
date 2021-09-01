/******************************************************************************/
/*!
\file   physics_system.cpp
\author Jeong Juyong
\par    email: jaykop.jy\@gmail.com
\date   2019/06/15(yy/mm/dd)

\description
Contains the methods of PhysicsSystem class
*/
/******************************************************************************/

#include <physics_system.hpp>
#include <rigidbody.hpp>
#include <object.hpp>
#include <transform.hpp>
#include <vec3.hpp>
#include <material.hpp>
#include <contact.hpp>

#include <iostream>

jeBegin

const int MAX_VERTICES = 64;
const float fCoS = 0.5f, fCoR = 0.7f, fCoF = 0.3f;
PhysicsSystem::Bodies PhysicsSystem::bodies_;
Material* PhysicsSystem::contactMaterial_ = nullptr;

void PhysicsSystem::add_rigidbody(RigidBody* rigidbody)
{
	bodies_.emplace_back(rigidbody);
}

void PhysicsSystem::initialize()
{
	contactMaterial_ = new Material;
	contactMaterial_->restitution = fCoR;
	contactMaterial_->friction = fCoF;
	contactMaterial_->staticFriction = fCoS; 
}

void PhysicsSystem::update(float dt)
{
	int size = static_cast<int>(bodies_.size());
	for (int i = 0; i < size - 1; ++i)
	{
		RigidBody::ColliderType type1 = bodies_[i]->collisionType_;
		if (type1 == RigidBody::ColliderType::NONE)
			continue;

		for (int j = i + 1; j < size; ++j)
		{
			RigidBody::ColliderType type2 = bodies_[j]->collisionType_;
			if (type2 == RigidBody::ColliderType::NONE
				|| (bodies_[i]->isStatic && bodies_[j]->isStatic))
				continue;

			vec3 N;
			float t = 1.0f;

			check_collision(bodies_[i], bodies_[j], N, t);
		}
	}

	for (const auto& b : bodies_)
	{
		if (b->isStatic)
		{
			// b->displacement_.set_zero();
			continue;
		}
		else if (b->gravity != 0.f)
		{
			vec3 gravity;
			gravity.y = -(b->gravity * b->mass);

			b->add_force(gravity, dt);
		}

		b->update(dt);
	}
}

void PhysicsSystem::close()
{
	delete contactMaterial_;
	contactMaterial_ = nullptr;

	//colliders_.clear();
	//colliders_.shrink_to_fit();

	bodies_.clear();
	//bodies_.shrink_to_fit();
}

bool PhysicsSystem::interval_intersect(const std::vector<vec3>& A, const std::vector<vec3>& B,
	const vec3& xAxis, const vec3& xOffset, const vec3& xVel, 
	const mat3& xOrient, float& tAxis, const float tMax)
{
	float min0, max0;
	float min1, max1;
	get_interval(A, xAxis * xOrient, min0, max0);
	get_interval(B, xAxis, min1, max1);

	float h = xOffset.dot(xAxis);
	min0 += h;
	max0 += h;

	float d0 = min0 - max1; // if overlapped, do < 0
	float d1 = min1 - max0; // if overlapped, d1 > 0

	if (d0 > 0.0f || d1 > 0.0f)
	{
		float v = xVel.dot(xAxis);

		// small velocity, so only the overlap test will be relevant. 
		if (fabs(v) < 0.0000001f)
			return false;

		float t0 = -d0 / v; // time of impact to d0 reaches 0
		float t1 = d1 / v; // time of impact to d0 reaches 1

		if (t0 > t1) { float temp = t0; t0 = t1; t1 = temp; }
		tAxis = (t0 > 0.0f) ? t0 : t1;

		if (tAxis < 0.0f || tAxis > tMax)
			return false;

		return true;
	}
	else
	{
		// overlap. get the interval, as a the smallest of |d0| and |d1|
		// return negative number to mark it as an overlap
		tAxis = (d0 > d1) ? d0 : d1;
		return true;
	}
}

// calculate the projection range of a polygon along an axis
void PhysicsSystem::get_interval(const std::vector<vec3>& vertices, const vec3& xAxis, float& min, float& max)
{
	int size = static_cast<int>(vertices.size());
	min = max = vertices[0].dot(xAxis);

	for (int i = 1; i < size; i++)
	{
		float d = vertices[i].dot(xAxis);

		if (d < min) 
			min = d; 
		else if (d > max) 
			max = d;
	}
}

bool PhysicsSystem::find_MTD(vec3* xAxis, float* taxis, int iAxes, vec3& N, float& t)
{
	// nope, find overlaps
	int mini = -1;

	t = 0.0f;
	for (int i = 0; i < iAxes; i++)
	{
		if (taxis[i] > 0 && taxis[i] > t)
		{
			mini = i;
			t = taxis[i];
			N = xAxis[i];
			N.normalize();
		}
	}

	// found one
	if (mini != -1)
		return true;

	// nope, find overlaps
	mini = -1;
	for (int i = 0; i < iAxes; i++)
	{
		float n = xAxis[i].length();
		xAxis[i].normalize();
		taxis[i] /= n;

		if (taxis[i] > t || mini == -1)
		{
			mini = i;
			t = taxis[i];
			N = xAxis[i];
		}
	}

	if (mini == -1)
		jeDebugPrint("Error\n");

	return (mini != -1);
}

float PhysicsSystem::calculate_mass(const std::vector<vec3>& vertices, float density)
{
	int size = static_cast<int>(vertices.size());
	if (size < 2)
		return 5.0f * density;

	float mass = 0.0f;

	for (int j = size - 1, i = 0; i < size; j = i, i++)
	{
		vec3 P0 = vertices[j];
		vec3 P1 = vertices[i];
		mass += (P0.cross(P1)).length();
	}
	if (size <= 2)
		mass = 10.0f;

	mass *= density * 0.5f;

	return mass;
}

float PhysicsSystem::calculate_inertia(const std::vector<vec3>& vertices, float mass)
{
	int size = static_cast<int>(vertices.size());
	if (size == 1)	return 0.0f;

	float denom = 0.0f;
	float numer = 0.0f;

	for (int j = size - 1, i = 0; i < size; j = i, i++)
	{
		vec3 P0 = vertices[j];
		vec3 P1 = vertices[i];

		float a = (P0.cross(P1)).length();
		float b = (P1.dot(P1) + P1.dot(P0) + P0.dot(P0));

		denom += (a * b);
		numer += a;
	}
	float inertia = (mass / 6.0f) * (denom / numer);

	return inertia;
}

bool project_point_on_segment(const vec3& V, const vec3& A, const vec3& B, vec3& W, float* pt = NULL)
{
	vec3 AV = V - A;
	vec3 AB = B - A;
	float t = (AV.dot(AB)) / (AB.dot(AB));

	if (t < 0.0f) t = 0.0f; else if (t > 1.0f) t = 1.0f;

	if (pt)	*pt = t;

	W = A + AB * t;

	return true;
}

bool convert_support_points_to_contacts(const vec3& N,
	std::vector<vec3>& S0, const int S0num, 
	std::vector<vec3>& S1, const int S1num,
	std::vector<vec3>& C0, std::vector<vec3>& C1,
	int& Cnum)
{
	Cnum = 0;

	if (S0num == 0 || S1num == 0)
		return false;

	if (S0num == 1 && S1num == 1)
	{
		C0[Cnum] = S0[0];
		C1[Cnum] = S1[0];
		Cnum++;
		return true;
	}

	vec3 xPerp(-N.y, N.x, 0.f);

	float min0 = S0[0].dot(xPerp);
	float max0 = min0;
	float min1 = S1[0].dot(xPerp);
	float max1 = min1;

	if (S0num == 2)
	{
		max0 = S0[1].dot(xPerp);

		if (max0 < min0)
		{
			std::swap(min0, max0);

			vec3 T = S0[0];
			S0[0] = S0[1];
			S0[1] = T;
		}
	}

	if (S1num == 2)
	{
		max1 = S1[1].dot(xPerp);

		if (max1 < min1)
		{
			std::swap(min1, max1);

			vec3 T = S1[0];
			S1[0] = S1[1];
			S1[1] = T;
		}
	}

	if (min0 > max1 || min1 > max0)
		return false;

	if (min0 > min1)
	{
		vec3 Pseg;
		if (project_point_on_segment(S0[0], S1[0], S1[1], Pseg))
		{
			C0[Cnum] = S0[0];
			C1[Cnum] = Pseg;
			Cnum++;
		}
	}
	else
	{
		vec3 Pseg;
		if (project_point_on_segment(S1[0], S0[0], S0[1], Pseg))
		{
			C0[Cnum] = Pseg;
			C1[Cnum] = S1[0];
			Cnum++;
		}
	}

	if (max0 != min0 && max1 != min1)
	{
		if (max0 < max1)
		{
			vec3 Pseg;
			if (project_point_on_segment(S0[1], S1[0], S1[1], Pseg))
			{
				C0[Cnum] = S0[1];
				C1[Cnum] = Pseg;
				Cnum++;
			}
		}
		else
		{
			vec3 Pseg;
			if (project_point_on_segment(S1[1], S0[0], S0[1], Pseg))
			{
				C0[Cnum] = Pseg;
				C1[Cnum] = S1[1];
				Cnum++;
			}
		}
	}
	return true;
}

vec3 transform(const vec3& Vertex, const vec3& P, const vec3& V, const mat3& xOrient, float t)
{
	vec3 T = P + (xOrient * Vertex);

	if (t > 0.0f)
		T += V * t;

	return T;
}

int find_support_points(const vec3& N, float t, const std::vector<vec3>& A, const vec3& PA, const vec3& VA, const mat3& OA, std::vector<vec3>& S)
{
	vec3 Norm = N * OA;

	float d[32];
	float dmin;
	dmin = d[0] = A[0].dot(Norm);
	int Anum = static_cast<int>(A.size());
	for (int i = 1; i < Anum; i++)
	{
		d[i] = A[i].dot(Norm);

		if (d[i] < dmin)
		{
			dmin = d[i];
		}
	}

	// we will limit the number of support points to only 2. 
	// if we have more than 2 support points, we take the extremums.
	int Snum = 0;
	const float threshold = 1.0E-3f;
	float s[2];
	float sign = false;

	vec3 Perp(-Norm.y, Norm.x, 0.f);

	for (int i = 0; i < Anum; i++)
	{
		if (d[i] < dmin + threshold)
		{
			vec3 Contact = transform(A[i], PA, VA, OA, t);

			float c = Contact.dot(Perp);

			if (Snum < 2)
			{
				s[Snum] = c;
				S[Snum] = Contact;
				Snum++;

				if (Snum > 1)
				{
					sign = (s[1] > s[0]);
				}
			}
			else
			{
				float& min = (sign) ? s[0] : s[1];
				float& max = (sign) ? s[1] : s[0];
				vec3& Min = (sign) ? S[0] : S[1];
				vec3& Max = (sign) ? S[1] : S[0];

				if (c < min)
				{
					min = c;
					Min = Contact;
				}
				else if (c > max)
				{
					max = c;
					Max = Contact;
				}
			}
		}
	}
	return Snum;
}

bool find_contacts(
	const std::vector<vec3>& A, const vec3& PA, const vec3& VA, const mat3& OA,
	const std::vector<vec3>& B, const vec3& PB, const vec3& VB, const mat3& OB,
	const vec3& N, float t, std::vector<vec3>& CA, std::vector<vec3>& CB, int& Cnum)
{
	std::vector<vec3> S0(4, 0);
	std::vector<vec3> S1(4, 0);
	int S0num = find_support_points(N, t, A, PA, VA, OA, S0);
	int S1num = find_support_points(-N, t, B, PB, VB, OB, S1);

	if (!convert_support_points_to_contacts(N, S0, S0num, S1, S1num, CA, CB, Cnum))
		return false;

	return true;
}

bool PhysicsSystem::check_collision(RigidBody* aBody, RigidBody* bBody, vec3& N, float& t)
{
	if (aBody->isStatic && bBody->isStatic) return false;

	aBody->init_vertices();
	bBody->init_vertices();

	const std::vector<vec3>& aVertices = aBody->vertices_;
	const std::vector<vec3>& bVertices = bBody->vertices_;

	int aSize = static_cast<int>(aVertices.size());
	int bSize = static_cast<int>(bVertices.size());

	if ((!aSize || !bSize) || (aSize < 1 && bSize < 1)) return false;

	const vec3& aPos = aBody->transform->position;
	const vec3& bPos = bBody->transform->position;
	const vec3& aVel = aBody->velocity;
	const vec3& bVel = bBody->velocity;
	mat3 aOrientation = aBody->orientation,
		bOrientation = bBody->orientation;

	mat3 aTransposed = aOrientation.transposed();
	mat3 bTransposed = bOrientation.transposed();

	vec3 relPos = (aPos - bPos) * bOrientation;
	vec3 relVel = (aVel - bVel) * bOrientation;
	mat3 relOrient = aOrientation * bTransposed;

	// All the separation axes
	vec3 xAxis[MAX_VERTICES]; // note : a maximum of 32 vertices per poly is supported
	float tAxis[MAX_VERTICES];
	int iAxes = 0;
	float fVel2 = relVel.dot(relVel);

	if (fVel2 > 0.000001f)
	{
		xAxis[iAxes] = vec3(-relVel.y, relVel.x, 0.f);

		if (!interval_intersect(aVertices, bVertices, xAxis[iAxes], relPos, relVel, relOrient, tAxis[iAxes], t))
		{
			return false;
		}
		iAxes++;
	}

	// test separation axes of A
	for (int j = aSize - 1, i = 0; i < aSize; j = i, i++)
	{
		vec3 E0 = aVertices[j];
		vec3 E1 = aVertices[i];
		vec3 E = E1 - E0;
		xAxis[iAxes] = relOrient * vec3(-E.y, E.x, 0.f);

		if (!interval_intersect(aVertices, bVertices, xAxis[iAxes], relPos, relVel, relOrient, tAxis[iAxes], t))
			return false;

		++iAxes;
	}

	// test separation axes of B
	for (int j = bSize - 1, i = 0; i < bSize; j = i, i++)
	{
		vec3 E0 = bVertices[j];
		vec3 E1 = bVertices[i];
		vec3 E = E1 - E0;
		xAxis[iAxes] = vec3(-E.y, E.x, 0.f);

		if (!interval_intersect(aVertices, bVertices, xAxis[iAxes], relPos, relVel, relOrient, tAxis[iAxes], t))
			return false;

		++iAxes;
	}

	// special case for segments
	if (bSize == 2)
	{
		vec3 E = bVertices[1] - bVertices[0];
		xAxis[iAxes] = E;

		if (!interval_intersect(aVertices, bVertices, xAxis[iAxes], relPos, relVel, relOrient, tAxis[iAxes], t))
		{
			return false;
		}

		++iAxes;
	}

	// special case for segments
	if (aSize == 2)
	{
		vec3 E = aVertices[1] - aVertices[0];
		xAxis[iAxes] = relOrient * E;

		if (!interval_intersect(aVertices, bVertices, xAxis[iAxes], relPos, relVel, relOrient, tAxis[iAxes], t))
		{
			return false;
		}

		++iAxes;
	}

	if (!find_MTD(xAxis, tAxis, iAxes, N, t))
		return false;

	// make sure the polygons gets pushed away from each other.
	if (N.dot(relPos) < 0.0f)
		N = -N;

	N = relOrient * N;

	std::vector<vec3> CA(4, 0);
	std::vector<vec3> CB(4, 0);
	int Cnum;

	find_contacts(
		aVertices, aPos, aVel, aOrientation,
		bVertices, bPos, bVel, bOrientation,
		N, t, CA, CB, Cnum);

	Contact xContact(CA, CB, Cnum, N, t, aBody, bBody);
	xContact.solve();
	return true;
}

jeEnd
