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

	bool isStatic = false;
	Transform* transform;

protected:

	virtual void add_to_system();
	virtual void remove_from_system();
	virtual void load(const rapidjson::Value& /*data*/) {};

private:

	vec3 displacement_;
	float mass_;

};

jeDeclareComponentBuilder(RigidBody);

jeEnd