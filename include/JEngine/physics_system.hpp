/******************************************************************************/
/*!
\file   physics_system.hpp
\author Jeong Juyong
\par    email: jaykop.jy\@gmail.com
\date   2019/06/15(yy/mm/dd)

\description
Contains the definition of PhysicsSystem class
*/
/******************************************************************************/

#pragma once
#include <macros.hpp>
#include <vector>
#include <vec3.hpp>

jeBegin

class PhysicsSystem {

	// Prevent to clone this class
	PhysicsSystem() = delete;
	~PhysicsSystem() = delete;

	jePreventClone(PhysicsSystem)

	friend class Scene;
	friend class Collider2D;
	friend class RigidBody;

	using Bodies = std::vector<RigidBody*>;
	using Colliders = std::vector<Collider2D*>;

public:

	static bool isCollided(Collider2D* a, Collider2D* b);

private:

	static void add_collider(Collider2D* collider);

	static void initialize();
	static void update(float dt);
	static void close();

	static bool IntervalIntersect(const std::vector<vec3>& A, const std::vector<vec3>& B, const vec3& xAxis, 
		const vec3& xOffset, float& tAxis);
	static void GetInterval(const std::vector<vec3>& vertices, const vec3& xAxis, float& min, float& max);
	static bool FindMTD(vec3* xAxis, float* taxis, int iAxes, vec3& N, float& t);

	static Colliders colliders_;
};

jeEnd