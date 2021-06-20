#pragma once
#include <component_builder.hpp>
#include <component.hpp>

jeBegin

class RigidBody : public Component {

	jeBaseFriends(RigidBody);
	friend class PhysicsSystem;

public:

	RigidBody(Object* owner);
	virtual ~RigidBody();

	bool isStatic = false;

protected:

	virtual void add_to_system() {};
	virtual void remove_from_system() {};
	virtual void load(const rapidjson::Value& /*data*/) {};

private:

};

jeDeclareComponentBuilder(RigidBody);

jeEnd