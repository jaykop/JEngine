#pragma once
#include <component_builder.hpp>
#include <component.hpp>

jeBegin

class Transform;
class Material : public Component {

	jeBaseFriends(Material);
	friend class PhysicsSystem;

public:

	Material(Object* owner);
	virtual ~Material();

	float friction = .2f;
	float restitution = .3f;
	float staticFriction = .4f;
	float separation = .5f;

protected:

	virtual void add_to_system();
	virtual void remove_from_system();
	virtual void load(const rapidjson::Value& /*data*/) {};

private:


};

jeDeclareComponentBuilder(Material);

jeEnd