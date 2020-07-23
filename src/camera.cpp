#include <camera.hpp>
#include <vec4.hpp>
#include <mat4.hpp>
#include <math_util.hpp>
#include <graphic_system.hpp>

jeBegin

vec3 Camera::worldUp_ = vec3(0.f, 1.f, 0.f);

jeDefineComponentBuilder(Camera);

void Camera::add_to_system() {
	GraphicSystem::add_camera(this);
}

void Camera::remove_from_system() {
	GraphicSystem::remove_camera(this);
}

void Camera::load(const rapidjson::Value& /*data*/) {
}

Camera::Camera(Object* owner) 
	: Component(owner), position(vec3::zero), near_(.1f), far_(1000.f),
	up_(vec3(0, 1, 0)), right_(vec3(1, 0, 0)), back_(vec3(0, 0, -1)),
	distance_(1.f), fovy_(45.f), zoom(0.f),
	yaw_(Math::deg_to_rad(-90.f)), roll_(0.f), pitch_(0.f), width_(2 * tanf(.5f * fovy_)), height_(width_ / aspect_),
	aspect_(GraphicSystem::width_ / GraphicSystem::height_)
{
	viewgeometry_.set(width_, height_, distance_);
}

void Camera::update(float /*dt*/)
{
	aspect_ = GraphicSystem::width_ / GraphicSystem::height_;
	width_ = 2 * tanf(.5f * fovy_);
	height_ = width_ / aspect_;
	viewgeometry_.set(width_, height_, distance_);
}

void Camera::set_yaw(float rad)
{
	yaw_ = rad;

	back_.x = cosf(yaw_) * cosf(pitch_);
	back_.y = sinf(pitch_);
	back_.z = sinf(yaw_) * cosf(pitch_);
	back_.normalize();

	// worldUp_ or up?
	right_ = (worldUp_).cross(-back_).normalized();
}

void Camera::set_pitch(float rad)
{
	pitch_ = rad;

	back_.x = cosf(yaw_) * cosf(pitch_);
	back_.y = sinf(pitch_);
	back_.z = sinf(yaw_) * cosf(pitch_);
	back_.normalize();

	up_ = (-back_).cross(right_).normalized();
}

float Camera::get_yaw() const { return yaw_; }

float Camera::get_pitch() const { return pitch_; }

// todo:
//void Camera::set_roll(float rad)
//{
//	roll_ = rad;
//
//	vec4 right = mat4::rotate(roll_, -back_) * vec4(right_, 1.f);
//	right_.set(right.x, right.y, right.z);
//	right_.normalize();
//
//	up_ = (-back_).cross(right_).normalized();
//}

// float Camera::get_roll() const { return roll_; }

const vec3& Camera::get_viewgeometry() const
{
	return viewgeometry_;
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

jeEnd