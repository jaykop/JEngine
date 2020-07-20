#include <camera.hpp>
#include <gl_manager.hpp>
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
	up_(vec3(0, 1, 0)), right_(vec3::zero), back_(vec3::zero), target(vec3::zero), 
	distance_(1.f), fovy_(45.f), zoom(0.f),
	yaw_(-90.f), roll_(0.f), pitch_(0.f), width_(2 * tanf(.5f * fovy_)), height_(width_ / aspect_),
	aspect_(GLManager::get_width() / GLManager::get_height())
{
	viewgeometry_.set(width_, height_, distance_);
}

void Camera::update()
{
	aspect_ = GLManager::get_width() / GLManager::get_height();
	width_ = 2 * tanf(.5f * fovy_);
	height_ = width_ / aspect_;

	viewgeometry_.set(width_, height_, distance_);

	back_ = (target-position).normalized();
	right_ = vec3::cross(-back_, up_).normalized();
	up_ = vec3::cross(right_, -back_).normalized();

	vec3 v = -back_;
	vec3 vp(v.x, v.y, 0.f);
	yaw_ = acosf(v.dot(vp) / (vec3::abs(v).dot(vec3::abs(vp))));
	vec3 vpp(0.f, v.y, 0.f);
	pitch_ = acosf(vp.dot(vpp) / (vec3::abs(vp).dot(vec3::abs(vp))));
}

void Camera::yaw(float degree)
{
	yaw_ = degree;
	mat4 rotate = mat4::rotate(Math::deg_to_rad(yaw_), up_);

	vec4 right(right_.x, right_.y, right_.z, 1.f);
	right = rotate * right;
	right_.set(right.x, right.y, right.z);

	vec4 back(back_.x, back_.y, back_.z, 1.f);
	back = rotate * back;
	back_.set(back.x, back.y, back.z);
	target = (-back_).normalized();
}

void Camera::pitch(float degree)
{
	pitch_ = degree;
	mat4 rotate = mat4::rotate(Math::deg_to_rad(pitch_), right_);

	vec4 up(up_.x, up_.y, up_.z, 1.f);
	up = rotate * up;
	up_.set(up.x, up.y, up.z);

	vec4 back(back_.x, back_.y, back_.z, 1.f);
	back = rotate * back;
	back_.set(back.x, back.y, back.z);
	target = (-back_).normalized();
}

void Camera::roll(float degree)
{
	roll_ = degree;
	mat4 rotate = mat4::rotate(Math::deg_to_rad(roll_), back_);

	vec4 right(right_.x, right_.y, right_.z, 1.f);
	right = rotate * right;
	right_.set(right.x, right.y, right.z);

	vec4 up(up_.x, up_.y, up_.z, 1.f);
	up = rotate * up;
	up_.set(up.x, up.y, up.z);
}

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