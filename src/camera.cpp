#include <camera.hpp>
#include <gl_manager.hpp>
#include <mat4.hpp>
#include <vec4.hpp>
#include <math_util.hpp>
#include <graphic_system.hpp>

jeBegin

jeDefineComponentBuilder(Camera);

void Camera::add_to_system() {
	GraphicSystem::add_camera(this);
}

void Camera::remove_from_system() {
	GraphicSystem::remove_camera(this);
}

void Camera::load(const rapidjson::Value& /*data*/) {
}

Camera::Camera(Object* owner) : Component(owner),
	position_(vec3::zero), near_(.1f), far_(1000.f),
	up_(vec3(0, 1, 0)), target_(vec3::zero), right_(vec3::zero), back_(vec3::zero),
	viewGeometry_(vec3::zero), distance_(1.f), fovy_(0.f), aspect_(0.f),
	width_(0.f), height_(0.f)
{
	set_camera(position_, vec3(0, 0, 1), up_, 45.f, GLManager::get_width() / GLManager::get_height(), 1.f);
}

void Camera::set_camera(const vec3& eye, const vec3& look, const vec3& up,
	float fov, float aspect, float distance)
{
	position_ = eye;
	right_ = look.cross(up).normalized();
	up_ = right_.cross(look).normalized();
	back_ = (-look).normalized();

	fovy_ = fov;
	aspect_ = aspect;
	distance_ = distance;
	width_ = 2 * tanf(.5f * fovy_);
	height_ = width_ / aspect_;

	viewGeometry_.set(width_, height_, distance_);
}

const vec3& Camera::get_viewGeometry() const
{
	return viewGeometry_;
}

float Camera::get_aspect() const
{
	return aspect_;
}

float Camera::get_distance() const
{
	return distance_;
}

const vec3& Camera::get_up() const
{
	return up_;
}

const vec3& Camera::get_right() const
{
	return right_;
}

const vec3& Camera::get_back() const
{
	return back_;
}

void Camera::yaw(float degree)
{
	mat4 rotate = mat4::rotate(Math::deg_to_rad(degree), up_);

	vec4 right(right_.x, right_.y, right_.z, 1.f);
	right = rotate * right;
	right_.set(right.x, right.y, right.z);

	vec4 back(back_.x, back_.y, back_.z, 1.f);
	back = rotate * back;
	back_.set(back.x, back.y, back.z);
}

void Camera::pitch(float degree)
{
	mat4 rotate = mat4::rotate(Math::deg_to_rad(degree), right_);

	vec4 up(up_.x, up_.y, up_.z, 1.f);
	up = rotate * up;
	up_.set(up.x, up.y, up.z);

	vec4 back(back_.x, back_.y, back_.z, 1.f);
	back = rotate * back;
	back_.set(back.x, back.y, back.z);
}

void Camera::roll(float degree)
{
	mat4 rotate = mat4::rotate(Math::deg_to_rad(degree), back_);

	vec4 right(right_.x, right_.y, right_.z, 1.f);
	right = rotate * right;
	right_.set(right.x, right.y, right.z);

	vec4 up(up_.x, up_.y, up_.z, 1.f);
	up = rotate * up;
	up_.set(up.x, up.y, up.z);
}

void Camera::zoom(float zoom)
{
	width_ *= zoom;
	height_ += zoom;
}

jeEnd