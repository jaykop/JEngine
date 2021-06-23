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

#include <iostream>

jeBegin

PhysicsSystem::Colliders PhysicsSystem::colliders_;
const int MAX_VERTICES = 64;

void PhysicsSystem::add_collider(Collider2D* collider)
{
	colliders_.emplace_back(collider);
}

void PhysicsSystem::initialize()
{
}

void PhysicsSystem::update(float /*dt*/)
{
	int size = static_cast<int>(colliders_.size());
	for (int i = 0; i < size - 1; ++i)
	{
		for (int j = 0; j < size - i - 1; ++j)
		{
			bool a = isCollided(colliders_[j], colliders_[j + 1]);
			if (a)
				std::cout << "collided\n";
			else
				std::cout << "not collided\n";
		}
	}
}

void PhysicsSystem::close()
{
	colliders_.clear();
	//colliders_.shrink_to_fit();
}

bool PhysicsSystem::isCollided(Collider2D* a, Collider2D* b)
{
	Object* aOnwer = a->get_owner();
	Object* bOnwer = b->get_owner();

	if (!aOnwer->get_component<RigidBody>()->isStatic)
		a->init_vertices();

	if (!bOnwer->get_component<RigidBody>()->isStatic)
		b->init_vertices();

	const std::vector<vec3>& aVertices = a->vertices_;
	const std::vector<vec3>& bVertices = b->vertices_;

	int aSize = static_cast<int>(aVertices.size());
	int bSize = static_cast<int>(bVertices.size());

	if (!aSize || !bSize) return false;

	vec3& aPos = aOnwer->get_component<Transform>()->position;
	vec3& bPos = bOnwer->get_component<Transform>()->position;

	vec3 dist = aPos - bPos;

	// All the separation axes
	vec3 xAxis[MAX_VERTICES]; // note : a maximum of 32 vertices per poly is supported
	float tAxis[MAX_VERTICES];
	int iAxes = 0;

	// test separation axes of A
	for (int j = aSize - 1, i = 0; i < aSize; j = i, i++)
	{
		vec3 E0 = aVertices[j];
		vec3 E1 = aVertices[i];
		vec3 E = E1 - E0;
		xAxis[iAxes] = vec3(-E.y, E.x, 0.f);

		if (!IntervalIntersect(aVertices, bVertices, xAxis[iAxes], dist, tAxis[iAxes]))
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

		if (!IntervalIntersect(aVertices, bVertices, xAxis[iAxes], dist, tAxis[iAxes]))
			return false;

		iAxes++;
	}

	vec3 N;
	float t;

	if (!FindMTD(xAxis, tAxis, iAxes, N, t))
		return false;

	// make sure the polygons gets pushed away from each other.
	if (N.dot(dist) < 0.0f)
		N = -N;

	aPos -= N * (t * 1.01f);
	bPos += N * (t * 1.01f);
	
	return true;
}

bool PhysicsSystem::IntervalIntersect(const std::vector<vec3>& A, const std::vector<vec3>& B,
	const vec3& xAxis, const vec3& xOffset, float& tAxis)
{
	float min0, max0;
	float min1, max1;
	GetInterval(A, xAxis, min0, max0);
	GetInterval(B, xAxis, min1, max1);

	float h = xOffset.dot(xAxis);
	min0 += h;
	max0 += h;

	float d0 = min0 - max1;
	float d1 = min1 - max0;

	if (d0 > 0.0f || d1 > 0.0f)
	{
		return false;
	}
	else
	{
		tAxis = (d0 > d1) ? d0 : d1;

		return true;
	}
}

void PhysicsSystem::GetInterval(const std::vector<vec3>& vertices, const vec3& xAxis, float& min, float& max)
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

bool PhysicsSystem::FindMTD(vec3* xAxis, float* taxis, int iAxes, vec3& N, float& t)
{
	// nope, find overlaps
	int mini = -1;

	t = 0.0f;
	N.set_zero();

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
