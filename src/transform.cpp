#include <transform.hpp>
#include <math_util.hpp>

using namespace Math;

jeBegin

jeDefineComponentBuilder(Transform);

Transform::Transform(Object* owner)
	: Component(owner),
	orientation(quat::identity)
	, position(0.0f, 0.0f, 0.0f)
	, scale(1.0f, 1.0f, 1.0f) 
{}

vec3 Transform::rotation_euler_rad(void) const
{
	return orientation.get_euler();
}

vec3 Transform::rotation_euler_deg(void) const
{
	return rotation_euler_rad() * radian_to_degree;
}

vec3 Transform::right(void)
{
	const mat4& m_to_w = model_to_world();
	return vec3(m_to_w.m00, m_to_w.m10, m_to_w.m20);
}

vec3 Transform::up(void)
{
	const mat4& m_to_w = model_to_world();
	return vec3(m_to_w.m01, m_to_w.m11, m_to_w.m21);
}

vec3 Transform::forward(void)
{
	const mat4& m_to_w = model_to_world();
	return vec3(m_to_w.m02, m_to_w.m12, m_to_w.m22);
}

void Transform::set_euler_rad(const vec3& rad)
{
	set_euler_rad(rad.x, rad.y, rad.z);
}

void Transform::set_euler_rad(float rad_x, float rad_y, float rad_z)
{
	orientation.set_euler(rad_x, rad_y, rad_z);
}

void Transform::set_euler_deg(const vec3& deg)
{
	set_euler_deg(deg.x, deg.y, deg.z);
}

void Transform::set_euler_deg(float deg_x, float deg_y, float deg_z)
{
	orientation.set_euler(deg_x * radian_to_degree, 
		deg_y * radian_to_degree, 
		deg_z * radian_to_degree);
}

mat4 Transform::model_to_world(void) const
{
	// combine rotation and translation
	mat4 m_to_w = orientation.to_mat4();
	m_to_w.m03 = position.x;
	m_to_w.m13 = position.y;
	m_to_w.m23 = position.z;

	// create scale mat
	mat4 s(scale.x, 0.0f, 0.0f, 0.0f,
		0.0f, scale.y, 0.0f, 0.0f,
		0.0f, 0.0f, scale.z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);

	m_to_w *= s;

	return m_to_w;
}

jeEnd