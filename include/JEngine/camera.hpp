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
	friend class Sprite;
	friend class Emitter;
	friend class InputHandler;
	friend class DebugRenderer;
	friend class GraphicSystem;

public:

	vec3 position, target;
	float fovy;

	void set_camera(const vec3& eye, const vec3& look, const vec3& up, float fov, float aspect, float distance);
	const vec3& get_viewGeometry() const;

	float get_aspect() const;
	float get_distance() const;

	const vec3& get_up() const;
	const vec3& get_right() const;
	const vec3& get_back() const;

	void yaw(float degree);
	void pitch(float degree);
	void roll(float degree);
	void zoom(float zoom);

protected:

	virtual void add_to_system();
	virtual void remove_from_system();
	virtual void load(const rapidjson::Value& data);

private:

	void update();

	vec3 up_, right_, back_, viewGeometry_;
	float distance_, aspect_, width_, height_, near_, far_;

	Camera(Object* pOwner);
	virtual ~Camera() {};
	Camera& operator=(const Camera& rhs);
};

jeEnd
