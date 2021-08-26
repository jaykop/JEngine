#pragma once
#include <component_builder.hpp>
#include <component.hpp>
#include <vec3.hpp>

jeBegin

class Transform;
class RigidBody : public Component {

	jeBaseFriends(RigidBody);
	friend class PhysicsSystem;

public:

	RigidBody(Object* owner);
	virtual ~RigidBody();

	void add_impulse(const vec3& force, float dt);
	float get_invMass() const;

	bool isStatic = false;
	Transform* transform;

	float friction;
	float restitution;
	float glue;
	float gravity;
	float mass;

protected:

	virtual void add_to_system();
	virtual void remove_from_system();
	virtual void load(const rapidjson::Value& /*data*/) {};

private:

	void process_collision(RigidBody* other, const vec3& N, float t);
	void process_overlap(RigidBody* other, const vec3& xMTD);

	vec3 displacement_;

};

jeDeclareComponentBuilder(RigidBody);

jeEnd