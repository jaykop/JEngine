#include <camera.hpp>
#include <gl_manager.hpp>
#include <mat4.hpp>
#include <vec4.hpp>
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
	up_(vec3(0, 1, 0)), right_(vec3::zero), back_(vec3::zero), front(vec3::zero), width_(0.f), height_(0.f),
	yaw_(0.f), roll_(0.f), pitch_(0.f), viewGeometry_(vec3::zero), distance_(1.f), fovy(0.f), aspect_(0.f)
{
	set_camera(position, -90.f, 0.f, 45.f, GLManager::get_width() / GLManager::get_height(), 1.f);
}

void Camera::set_camera(const vec3& eye, float yaw, float pitch,
	float fov, float aspect, float distance)
{
	yaw_ = yaw;
	pitch_ = pitch;
	position = eye;
	fovy = fov;
	aspect_ = aspect;
	distance_ = distance;
	width_ = 2 * tanf(.5f * fovy);
	height_ = width_ / aspect_;

	viewGeometry_.set(width_, height_, distance_);

	float rYaw = Math::deg_to_rad(yaw_);
	float rPitch = Math::deg_to_rad(pitch_);

	vec3 forward;
	forward.x = cosf(rYaw) * cosf(rPitch);
	forward.y = sinf(rPitch);
	forward.z = sinf(rYaw) * cosf(rPitch);
	front = forward.normalize();
	back_ = (-front).normalized();
	right_ = vec3::cross(front, worldUp_).normalized();
	up_ = vec3::cross(right_, front).normalized();
}

void Camera::update()
{
	aspect_ = GLManager::get_width() / GLManager::get_height();
	width_ = 2 * tanf(.5f * fovy);
	height_ = width_ / aspect_;

	viewGeometry_.set(width_, height_, distance_);

	front.normalize();
	back_ = (-front).normalized();
	pitch_ = asin(back_.y);
	yaw_ = atan2(back_.x, back_.z);

	right_ = vec3::cross(front, up_).normalized();
	up_ = vec3::cross(right_, front).normalized();
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
	front = (-back_).normalized();
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
	front = (-back_).normalized();
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

void Camera::zoom(float zoom)
{
	width_ *= zoom;
	height_ += zoom;
}

mat4 Camera::get_viewmatrix() const
{
	mat4 toReturn;

	toReturn.m[0][0] = right_.x;
	toReturn.m[0][1] = right_.y;
	toReturn.m[0][2] = right_.z;
	toReturn.m[0][3] = (-right_).dot(position);

	toReturn.m[1][0] = up_.x;
	toReturn.m[1][1] = up_.y;
	toReturn.m[1][2] = up_.z;
	toReturn.m[1][3] = (-up_).dot(position);

	toReturn.m[2][0] = back_.x;
	toReturn.m[2][1] = back_.y;
	toReturn.m[2][2] = back_.z;
	toReturn.m[2][3] = (-back_).dot(position);

	toReturn.m[3][3] = 1.f;

	return toReturn;
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

jeEnd