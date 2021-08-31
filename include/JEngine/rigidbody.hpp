#pragma once
#include <component_builder.hpp>
#include <component.hpp>
#include <vec3.hpp>
#include <mat4.hpp>

jeBegin

class Transform;
class RigidBody : public Component {

	jeBaseFriends(RigidBody);
	friend class PhysicsSystem;

public:
	
	enum class ColliderType {
		NONE,
		CIRCLE,
		RECT,
	};

	RigidBody(Object* owner);
	virtual ~RigidBody();

	bool is_unmovable();

	void add_impulse(const vec3& force, float dt);
	float get_invMass() const;
	float get_invInertia();

	void add_force(const vec3& f);
	void add_force(const vec3& f, const vec3& p);

	void set_density(float density);
	void set_orientation(float orientation);

	bool isStatic = false;
	Transform* transform;

	ColliderType  coliisionType_ = ColliderType::NONE;

	float friction;
	float restitution;
	float glue;
	float gravity;
	float mass;
	float inertia;
	float angVelocity;
	float angOrientation;

	vec3 velocity;
	mat4 orientation;

protected:

	virtual void add_to_system();
	virtual void remove_from_system();
	virtual void load(const rapidjson::Value& /*data*/) {};

private:

	void initialize();
	void update(float dt);

	void process_collision(RigidBody* other, const vec3& N, float t);
	void process_overlap(RigidBody* other, const vec3& xMTD);

	void init_vertices();

	float density_;
	float netTorque;

	vec3 netForce;
	vec3 displacement_;

	std::vector<vec3> vertices_;
};

jeDeclareComponentBuilder(RigidBody);

jeEnd