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
		GHOST,
		CIRCLE,
		RECT,
	};

	RigidBody(Object* owner);
	virtual ~RigidBody();

	float get_invMass() const;
	float get_invInertia();

	void add_force(const vec3& f);
	void add_force(const vec3& f, const vec3& p);

	void set_density(float density);
	void set_orientation(float orientation);

	void remove_force();

	bool is_collided();
	bool is_collided_with(RigidBody* body);
	std::vector<RigidBody*> get_collided() const;

	bool isStatic = false;
	Transform* transform;

	ColliderType  collisionType_ = ColliderType::NONE;

	float friction;
	float restitution;
	float glue;
	float gravity;
	float mass;
	float inertia;
	float angVelocity;
	float angOrientation;

	vec3 velocity;

protected:

	virtual void add_to_system();
	virtual void remove_from_system();
	virtual void load(const rapidjson::Value& /*data*/) {};

private:

	void initialize();
	void update(float dt);

	void init_vertices();

	void add_collided(RigidBody* body);
	void clear_collided();

	float density_;
	float netTorque;

	vec3 netForce;

	std::vector<vec3> vertices_;
	std::vector<RigidBody*> collided_;
};

jeDeclareComponentBuilder(RigidBody);

jeEnd