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
#include <collider_2d.hpp>
#include <rigidbody.hpp>
#include <object.hpp>
#include <transform.hpp>
#include <vec3.hpp>

#include <iostream>

jeBegin

PhysicsSystem::Colliders PhysicsSystem::colliders_;
PhysicsSystem::Bodies PhysicsSystem::bodies_;
const int MAX_VERTICES = 64;

void PhysicsSystem::add_collider(Collider2D* collider)
{
	colliders_.emplace_back(collider);
}

void PhysicsSystem::add_rigidbody(RigidBody* rigidbody)
{
	bodies_.emplace_back(rigidbody);
}

void PhysicsSystem::initialize()
{
}

void PhysicsSystem::update(float dt)
{
	int size = static_cast<int>(colliders_.size());
	for (int i = 0; i < size - 1; ++i)
	{
		for (int j = 0; j < size - i - 1; ++j)
		{
			vec3 N;
			float t = 1.0f;

			RigidBody* aBody = colliders_[j]->get_owner()->get_component<RigidBody>();
			RigidBody* bBody = colliders_[j + 1]->get_owner()->get_component<RigidBody>();

			if (is_collided(colliders_[j], colliders_[j + 1], aBody, bBody, N, t))
			{
				if (t < 0.f)
					aBody->process_overlap(bBody, N * -t);
				else
					aBody->process_collision(bBody, N, t);
			}
		}
	}

	for (const auto& b : bodies_)
	{
		if (b->isStatic)
		{
			b->displacement_.set_zero();
			continue;
		}
		
		b->transform->position += b->displacement_;
	}
}

void PhysicsSystem::close()
{
	colliders_.clear();
	//colliders_.shrink_to_fit();

	bodies_.clear();
	//bodies_.shrink_to_fit();
}

bool PhysicsSystem::is_collided(Collider2D* a, Collider2D* b, RigidBody* aBody, RigidBody* bBody, vec3& N, float& t)
{
	if (!aBody->isStatic)
		a->init_vertices();

	if (!bBody->isStatic)
		b->init_vertices();

	const std::vector<vec3>& aVertices = a->vertices_;
	const std::vector<vec3>& bVertices = b->vertices_;

	int aSize = static_cast<int>(aVertices.size());
	int bSize = static_cast<int>(bVertices.size());

	if (!aSize || !bSize) return false;

	const vec3& aPos = a->transform->position;
	const vec3& bPos = b->transform->position;
	const vec3& aDis = aBody->displacement_;
	const vec3& bDis = bBody->displacement_;
	mat3 aOrientation = a->transform->orientation.to_mat3(), bOrientation = b->transform->orientation.to_mat3();

	mat3 aTransposed = aOrientation.transposed();
	mat3 bTransposed = bOrientation.transposed();

	vec3 relPos = bTransposed * (aPos - bPos);
	vec3 relDis = bTransposed * (aDis - bDis);
	mat3 relOrient = bTransposed * aTransposed;

	// All the separation axes
	vec3 xAxis[MAX_VERTICES]; // note : a maximum of 32 vertices per poly is supported
	float tAxis[MAX_VERTICES];
	int iAxes = 0;
	float fVel2 = relDis.dot(relDis);

	if (fVel2 > 0.000001f)
	{
		if (!interval_intersect(aVertices, bVertices, xAxis[iAxes], relPos, relDis, relOrient, tAxis[iAxes], t))
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

		if (!interval_intersect(aVertices, bVertices, xAxis[iAxes], relPos, relDis, relOrient, tAxis[iAxes], t))
			return false;

		iAxes++;
	}

	// test separation axes of B
	for (int j = bSize - 1, i = 0; i < bSize; j = i, i++)
	{
		vec3 E0 = bVertices[j];
		vec3 E1 = bVertices[i];
		vec3 E = E1 - E0;
		xAxis[iAxes] = vec3(-E.y, E.x, 0.f);

		if (!interval_intersect(aVertices, bVertices, xAxis[iAxes], relPos, relDis, relOrient, tAxis[iAxes], t))
			return false;

		iAxes++;
	}

	// special case for segments
	if (bSize == 2)
	{
		vec3 E = bVertices[1] - bVertices[0];
		xAxis[iAxes] = E;

		if (!interval_intersect(aVertices, bVertices, xAxis[iAxes], relPos, relDis, relOrient, tAxis[iAxes], t))
		{
			return false;
		}

		iAxes++;
	}

	// special case for segments
	if (aSize == 2)
	{
		vec3 E = aVertices[1] - aVertices[0];
		xAxis[iAxes] = relOrient * E;

		if (!interval_intersect(aVertices, bVertices, xAxis[iAxes], relPos, relDis, relOrient, tAxis[iAxes], t))
		{
			return false;
		}

		iAxes++;
	}

	if (!find_MTD(xAxis, tAxis, iAxes, N, t))
		return false;

	// make sure the polygons gets pushed away from each other.
	if (N.dot(relPos) < 0.0f)
		N = -N;
	
	N = relOrient * N;

	return true;
}

bool PhysicsSystem::interval_intersect(const std::vector<vec3>& A, const std::vector<vec3>& B,
	const vec3& xAxis, const vec3& xOffset, const vec3& xVel, 
	const mat3& xOrient, float& tAxis, const float tMax)
{
	float min0, max0;
	float min1, max1;
	get_interval(A, xOrient.transposed() * xAxis, min0, max0);
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
		if (taxis[i] > 0)
		{
			if (taxis[i] > t)
			{
				mini = i;
				t = taxis[i];
				N = xAxis[i];
				N.normalize();
			}
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

jeEnd
