#pragma once
#include <component_builder.hpp>
#include <component.hpp>
#include <vec3.hpp>

jeBegin

class Object;

jeDeclareComponentBuilder(Camera);

class Camera : public Component
{
	jeBaseFriends(Camera);

	friend class Text;
	friend class Light;
	friend class Sprite;
	friend class Emitter;
	friend class InputHandler;
	friend class DebugRenderer;
	friend class GraphicSystem;

public:

	vec3 position;
	float zoom;

	const vec3& get_viewgeometry() const;

	float get_aspect() const;
	float get_distance() const;

	const vec3& get_up() const;
	const vec3& get_right() const;
	const vec3& get_back() const;

	void set_yaw(float degree);
	void set_pitch(float degree);
	void set_roll(float degree);

	float get_yaw() const;
	float get_pitch() const;
	float get_roll() const;

protected:

	virtual void add_to_system();
	virtual void remove_from_system();
	virtual void load(const rapidjson::Value& data);

private:

	void update(float dt);

	static vec3 worldUp_;

	vec3 up_, right_, back_, viewgeometry_;
	float distance_, aspect_, fovy_, width_, height_, near_, far_;
	float yaw_, roll_, pitch_;

	Camera(Object* pOwner);
	virtual ~Camera() {};
	Camera& operator=(const Camera& rhs);
};

jeEnd
