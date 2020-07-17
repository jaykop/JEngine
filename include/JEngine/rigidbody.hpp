#pragma once
#include <component_builder.hpp>
#include <component.hpp>

jeBegin

class Object;
jeDeclareComponentBuilder(RigidBody);

class RigidBody : public Component {

	jeBaseFriends(RigidBody);

public:



protected:

	virtual void add_to_system() {};
	virtual void remove_from_system() {};
	virtual void load(const rapidjson::Value& /*data*/) {};

private:

	RigidBody(Object* owner);
	virtual ~RigidBody() {};

};

jeEnd