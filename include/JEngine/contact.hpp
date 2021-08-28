#pragma once
#include <component_builder.hpp>
#include <component.hpp>

jeBegin

class Transform;
class Contact : public Component {

	jeBaseFriends(Contact);
	friend class PhysicsSystem;

public:

	Contact(Object* owner);
	virtual ~Contact();

protected:

	virtual void add_to_system();
	virtual void remove_from_system();
	virtual void load(const rapidjson::Value& /*data*/) {};

private:


};

jeDeclareComponentBuilder(Contact);

jeEnd