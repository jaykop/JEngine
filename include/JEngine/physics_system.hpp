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

struct vec3;
struct mat3;

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

	static bool is_collided(Collider2D* aCollider, Collider2D* bCollider,
		RigidBody* aBody, RigidBody* bBody, vec3& N, float& t);

private:

	static void add_collider(Collider2D* collider);
	static void add_rigidbody(RigidBody* rigidbody);

	static void initialize();
	static void update(float dt);
	static void close();

	static bool interval_intersect(const std::vector<vec3>& A, const std::vector<vec3>& B, const vec3& xAxis, 
		const vec3& xOffset, const vec3& xVel, const mat3& xOri, float& tAxis, const float tMax);
	static void get_interval(const std::vector<vec3>& vertices, const vec3& xAxis, float& min, float& max);
	static bool find_MTD(vec3* xAxis, float* taxis, int iAxes, vec3& N, float& t);

	static Colliders colliders_;
	static Bodies bodies_;
};

jeEnd