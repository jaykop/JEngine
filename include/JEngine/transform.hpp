#pragma once
#include <component_builder.hpp>
#include <component.hpp>
#include <vec3.hpp>
#include <mat4.hpp>
#include <quat.hpp>

jeBegin

class Object;
jeDeclareComponentBuilder(Transform);
class Transform : public Component {

	jeBaseFriends(Transform);

public:

	// public members
	quat orientation;
	vec3 position;
	vec3 scale;

	// getters
	vec3 rotation_euler_rad(void) const;
	vec3 rotation_euler_deg(void) const;

	// orientation
	vec3 right(void);
	vec3 up(void);
	vec3 forward(void);

	// setters
	void set_euler_rad(const vec3& rad);
	void set_euler_rad(float rad_x, float rad_y, float rad_z);
	void set_euler_deg(const vec3& deg);
	void set_euler_deg(float deg_x, float deg_y, float deg_z);

	// matrix transformation
	mat4 model_to_world(void) const;

protected:

	virtual void add_to_system() {};
	virtual void remove_from_system() {};
	virtual void load(const rapidjson::Value& /*data*/) {};

private:

	Transform(Object* owner);
	virtual ~Transform() {};

	Transform& operator=(const Transform& rhs);

};

jeEnd