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

protected:

	virtual void add_to_system();
	virtual void remove_from_system();
	virtual void load(const rapidjson::Value& /*data*/) {};

private:


};

jeDeclareComponentBuilder(Material);

jeEnd