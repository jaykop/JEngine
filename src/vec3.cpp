#include <cmath>
#include <vec3.hpp>
#include <math_util.hpp>

using namespace Math;

const vec3 vec3::zero = vec3(0.0f, 0.0f, 0.0f);
const vec3 vec3::one = vec3(1.f, 1.f, 1.f);

vec3::vec3(void)
	:x(0.f), y(0.f), z(0.f)
{}

vec3::vec3(float _x, float _y, float _z)
	: x(_x)
	, y(_y)
	, z(_z)
{}

vec3::vec3(const vec3& v3)
	: x(v3.x)
	, y(v3.y)
	, z(v3.z)
{}

void vec3::set(const vec3& v3)
{
	x = v3.x;
	y = v3.y;
	z = v3.z;
}

void vec3::set(float f)
{
	x = y = z = f;
}

void vec3::set(float f1, float f2, float f3)
{
	x = f1;
	y = f2;
	z = f3;
}

void vec3::set_zero(void)
{
	x = y = z = 0.0f;
}

void vec3::set_one(void)
{
	x = y = z = 1.0f;
}

float vec3::length(void) const
{
	return (sqrt(length_sq()));
}

float vec3::length_sq(void) const
{
	return (dot(*this));
}

vec3& vec3::normalize(void)
{
	float len = length();
	*this /= len;
	return (*this);
}

vec3 vec3::normalized(void) const
{
	float len = length();
	return (*this / len);
}

float vec3::dot(const vec3& rhs) const
{
	return (x * rhs.x + y * rhs.y + z * rhs.z);
}

vec3 vec3::cross(const vec3& rhs) const
{
	return vec3(y * rhs.z - z * rhs.y,
		z * rhs.x - x * rhs.z,
		x * rhs.y - y * rhs.x);
}

vec3 vec3::operator+(const vec3& rhs) const
{
	return (vec3(x + rhs.x, y + rhs.y, z + rhs.z));
}

vec3 vec3::operator*(const vec3& rhs) const
{
	return (vec3(x * rhs.x, y * rhs.y, z * rhs.z));
}

vec3& vec3::operator+=(const vec3& rhs)
{
	x += rhs.x;
	y += rhs.y;
	z += rhs.z;

	return (*this);
}

vec3 vec3::operator-(const vec3& rhs) const
{
	return (vec3(x - rhs.x, y - rhs.y, z - rhs.z));
}

vec3& vec3::operator-=(const vec3& rhs)
{
	x -= rhs.x;
	y -= rhs.y;
	z -= rhs.z;

	return (*this);
}

vec3 vec3::operator*(float rhs) const
{
	return (vec3(x * rhs, y * rhs, z * rhs));
}

vec3& vec3::operator*=(float rhs)
{
	x *= rhs;
	y *= rhs;
	z *= rhs;

	return (*this);
}

vec3 vec3::operator/(float rhs) const
{
	return (vec3(x / rhs, y / rhs, z / rhs));
}

vec3& vec3::operator/=(float rhs)
{
	x /= rhs;
	y /= rhs;
	z /= rhs;

	return (*this);
}

vec3 vec3::operator-(void) const
{
	return (vec3(-x, -y, -z));
}

bool vec3::operator==(const vec3& rhs) const
{
	return (float_equal(x, rhs.x) &&
		float_equal(y, rhs.y) &&
		float_equal(z, rhs.z));
}

bool vec3::operator!=(const vec3& rhs) const
{
	return !(*this == rhs);
}

float vec3::distance(const vec3& a, const vec3& b)
{
	vec3 diff = b - a;
	return (diff.length());
}

float vec3::distance_sq(const vec3& a, const vec3& b)
{
	vec3 diff = b - a;
	return (diff.length_sq());
}

vec3 vec3::cross(const vec3& lhs, const vec3& rhs)
{
	return lhs.cross(rhs);
}
