#include <math_util.hpp>
#include <quat.hpp>

using namespace Math;

const quat quat::identity(0.0f, 0.0f, 0.0f, 1.0f);

quat::quat(void) : x(0.f), y(0.f), z(0.f), w(0.f)
{}

quat::quat(float f1, float f2, float f3, float f4)
	: x(f1)	, y(f2)	, z(f3)	, w(f4)
{}

quat::quat(float radians, const vec4& axis)
{
	set(radians, axis);
}

quat::quat(float radians, const vec3& axis)
{
	set(radians, axis);
}

quat::quat(const mat4& m)
{
	set(m);
}

quat::quat(const mat3& m)
{
	set(m);
}

quat::quat(const quat& q)
	:x(q.x), y(q.y), z(q.z), w(q.w)
{}

quat& quat::normalize(void)
{
	float length = dot(*this);

	v /= length;
	s /= length;

	return *this;
}

quat quat::normalized(void) const
{
	return quat(*this).normalize();
}

quat& quat::invert(void)
{
	v = -v;
	return *this;
}

quat quat::inverted(void) const
{
	return quat(-x, -y, -z, w);
}

float quat::dot(const quat& rhs) const
{
	return (w * rhs.w + v.dot(rhs.v));
}

void quat::set_explicit(float _x, float _y, float _z, float _w)
{
	x = _x;
	y = _y;
	z = _z;
	w = _w;
}

void quat::set_explicit(float _s, const vec3& _v)
{
	set_explicit(_s, _v.x, _v.y, _v.z);
}

void quat::set(float angle_rad, float axis_x, float axis_y, float axis_z)
{
	float length = sqrtf(axis_x * axis_x + axis_y * axis_y + axis_z * axis_z);
	float half_angle = angle_rad / 2.0f;
	float sin_theta2 = sinf(half_angle);

	x = sin_theta2 * axis_x / length;
	y = sin_theta2 * axis_y / length;
	z = sin_theta2 * axis_z / length;
	w = cosf(half_angle);
}

void quat::set(float angle_rad, const vec4& axis)
{
	set(angle_rad, axis.x, axis.y, axis.z);
}

void quat::set(float angle_rad, const vec3& axis)
{
	set(angle_rad, axis.x, axis.y, axis.z);
}

void quat::set_euler(float rad_x, float rad_y, float rad_z)
{
	float cos_x = cosf(rad_x / 2.0f);
	float cos_y = cosf(rad_y / 2.0f);
	float cos_z = cosf(rad_z / 2.0f);

	float sin_x = sinf(rad_x / 2.0f);
	float sin_y = sinf(rad_y / 2.0f);
	float sin_z = sinf(rad_z / 2.0f);

	w = cos_y * cos_x * cos_z + sin_y * sin_x * sin_z;
	x = cos_y * sin_x * cos_z + sin_y * cos_x * sin_z;
	y = sin_y * cos_x * cos_z - cos_y * sin_x * sin_z;
	z = cos_y * cos_x * sin_z - sin_y * sin_x * cos_z;
}

void quat::set_euler(const vec3& rad)
{
	set_euler(rad.x, rad.y, rad.z);
}

vec3 quat::get_euler(void) const
{
	vec3 euler;
	float sin_x = -2.0f * (y * z - w * x);

	// check for gimbal lock
	if (float_equal(sin_x, 1.0f))
	{
		euler.x = pi_2 * sin_x;
		euler.y = atan2f(-x * z + w * y, 0.5f - y * y - z * z);
		euler.z = 0.0f;
	}
	else
	{
		euler.x = asinf(sin_x);
		euler.y = atan2f(x * z + w * y, 0.5f - x * x - y * y);
		euler.z = atan2f(x * y + w * z, 0.5f - x * x - z * z);
	}

	return euler;
}

void quat::set(const mat4& m)
{
	float trace = m.m00 + m.m11 + m.m22;

	if (trace > 0)
	{
		float sq = sqrtf(trace + 1.0f) * 2.0f;
		w = 0.25f * sq;
		x = (m.m21 - m.m12) / sq;
		y = (m.m02 - m.m20) / sq;
		z = (m.m10 - m.m01) / sq;
	}
	else if (m.m00 > m.m11 && m.m00 > m.m22)
	{
		float sq = sqrtf(1.0f + m.m00 - m.m11 - m.m22) * 2.0f;
		w = (m.m21 - m.m12) / sq;
		x = 0.25f * sq;
		y = (m.m01 + m.m10) / sq;
		z = (m.m02 + m.m20) / sq;
	}
	else if (m.m11 > m.m22)
	{
		float sq = sqrtf(1.0f + m.m11 - m.m00 - m.m22) * 2.0f;
		w = (m.m02 - m.m20) / sq;
		x = (m.m01 + m.m10) / sq;
		y = 0.25f * sq;
		z = (m.m12 + m.m21) / sq;
	}
	else
	{
		float sq = sqrtf(1.0f + m.m22 - m.m00 - m.m11) * 2.0f;
		w = (m.m10 - m.m01) / sq;
		x = (m.m02 + m.m20) / sq;
		y = (m.m12 + m.m21) / sq;
		z = 0.25f * sq;
	}
}

void quat::set(const mat3& m)
{
	float trace = m.m00 + m.m11 + m.m22;

	if (trace > 0)
	{
		float sq = sqrtf(trace + 1.0f) * 2.0f;
		w = 0.25f * sq;
		x = (m.m21 - m.m12) / sq;
		y = (m.m02 - m.m20) / sq;
		z = (m.m10 - m.m01) / sq;
	}
	else if (m.m00 > m.m11 && m.m00 > m.m22)
	{
		float sq = sqrtf(1.0f + m.m00 - m.m11 - m.m22) * 2.0f;
		w = (m.m21 - m.m12) / sq;
		x = 0.25f * sq;
		y = (m.m01 + m.m10) / sq;
		z = (m.m02 + m.m20) / sq;
	}
	else if (m.m11 > m.m22)
	{
		float sq = sqrtf(1.0f + m.m11 - m.m00 - m.m22) * 2.0f;
		w = (m.m02 - m.m20) / sq;
		x = (m.m01 + m.m10) / sq;
		y = 0.25f * sq;
		z = (m.m12 + m.m21) / sq;
	}
	else
	{
		float sq = sqrtf(1.0f + m.m22 - m.m00 - m.m11) * 2.0f;
		w = (m.m10 - m.m01) / sq;
		x = (m.m02 + m.m20) / sq;
		y = (m.m12 + m.m21) / sq;
		z = 0.25f * sq;
	}
}

mat4 quat::to_mat4(void) const
{
	float xx = 2 * x * x;
	float yy = 2 * y * y;
	float zz = 2 * z * z;
	float xy = 2 * x * y;
	float xz = 2 * x * z;
	float yz = 2 * y * z;
	float wx = 2 * w * x;
	float wy = 2 * w * y;
	float wz = 2 * w * z;

	return mat4(1.0f - (yy + zz), xy - wz, xz + wy, 0.0f,
		xy + wz, 1.0f - (xx + zz), yz - wx, 0.0f,
		xz - wy, yz + wx, 1.0f - (xx + yy), 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
}

mat3 quat::to_mat3(void) const
{
	float xx = 2 * x * x;
	float yy = 2 * y * y;
	float zz = 2 * z * z;
	float xy = 2 * x * y;
	float xz = 2 * x * z;
	float yz = 2 * y * z;
	float wx = 2 * w * x;
	float wy = 2 * w * y;
	float wz = 2 * w * z;

	return mat3(1.0f - (yy + zz), xy - wz, xz + wy,
		xy + wz, 1.0f - (xx + zz), yz - wx,
		xz - wy, yz + wx, 1.0f - (xx + yy));
}

void quat::to_matrix(mat4& out_matrix) const
{
	out_matrix = to_mat4();
}

void quat::to_matrix(mat3& out_matrix) const
{
	out_matrix = to_mat3();
}

quat quat::operator*(const quat& rhs) const
{
	vec3 v_res = v.cross(rhs.v) + (rhs.v * s) + (v * rhs.s);
	return quat(v_res.x, v_res.y, v_res.z, s * rhs.s - v.dot(rhs.v));
}

quat& quat::operator*=(const quat& rhs)
{
	*this = *this * rhs;
	return *this;
}

quat quat::operator-(void) const
{
	return quat(-x, -y, -z, -w);
}

bool quat::operator==(const quat& rhs) const
{
	return (float_equal(s, rhs.s) && v == rhs.v);
}

bool quat::operator!=(const quat& rhs) const
{
	return !(*this == rhs);
}
