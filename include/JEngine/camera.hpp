#pragma once
#include <component_builder.hpp>
#include <component.hpp>
#include <vec3.hpp>
#include <mat4.hpp>

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

	vec3 position, front;
	float fovy;

	void set_camera(const vec3& eye, float yaw, float pitch, 
		float fov, float aspect, float distance);

	mat4 get_viewmatrix() const;
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

	static vec3 worldUp_;

	vec3 up_, right_, back_, viewGeometry_;
	float distance_, aspect_, width_, height_, near_, far_;
	float yaw_, roll_, pitch_;

	Camera(Object* pOwner);
	virtual ~Camera() {};
	Camera& operator=(const Camera& rhs);
};

jeEnd
