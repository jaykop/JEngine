#include <vec4.hpp>
#include <math_util.hpp>
#include <cmath>

using namespace Math;

const vec4 vec4::zero = vec4(0.0f, 0.0f, 0.0f, 0.0f);

vec4::vec4(void)
{
}

vec4::vec4(float _x, float _y, float _z, float _w)
	: x(_x)
	, y(_y)
	, z(_z)
	, w(_w)
{
}

vec4::vec4(const vec3& v, float _w)
	: x(v.x)
	, y(v.y)
	, z(v.z)
	, w(_w)
{
}

void vec4::set(float f1, float f2, float f3, float f4)
{
	x = f1;
	y = f2;
	z = f3;
	w = f4;
}

void vec4::set(float f)
{
	x = y = z = w = f;
}

void vec4::set_zero(void)
{
	x = y = z = w = 0.0f;
}

void vec4::set_one(void)
{
	x = y = z = w = 1.0f;
}

float vec4::length(void) const
{
	return sqrt(length_sq());
}

float vec4::length_sq(void) const
{
	return (x * x + y * y + z * z + w * w);
}

vec4& vec4::normalize(void)
{
	float len = length();

	*this /= len;
	return (*this);
}

vec4 vec4::normalized(void) const
{
	float len = length();

	return (*this / len);
}

vec3 vec4::to_vec3(void) const
{
	return vec3(x, y, z);
}

float vec4::dot(const vec4& rhs) const
{
	return (x * rhs.x + y * rhs.y + z * rhs.z + w * rhs.w);
}

vec4 vec4::cross(const vec4& rhs) const
{
	return vec4(y * rhs.z - z * rhs.y,
		z * rhs.x - x * rhs.z,
		x * rhs.y - y * rhs.x,
		0.0f);
}

vec4 vec4::operator+(const vec4& rhs) const
{
	return vec4(x + rhs.x,
		y + rhs.y,
		z + rhs.z,
		w + rhs.w);
}

vec4& vec4::operator+=(const vec4& rhs)
{
	set(x + rhs.x,
		y + rhs.y,
		z + rhs.z,
		w + rhs.w);

	return (*this);
}

vec4 vec4::operator-(const vec4& rhs) const
{
	return vec4(x - rhs.x,
		y - rhs.y,
		z - rhs.z,
		w - rhs.w);
}

vec4& vec4::operator-=(const vec4& rhs)
{
	set(x - rhs.x,
		y - rhs.y,
		z - rhs.z,
		w - rhs.w);

	return (*this);
}

vec4 vec4::operator*(float rhs) const
{
	return vec4(x * rhs,
		y * rhs,
		z * rhs,
		w * rhs);
}

vec4& vec4::operator*=(float rhs)
{
	set(x * rhs,
		y * rhs,
		z * rhs,
		w * rhs);

	return (*this);
}

vec4 vec4::operator/(float rhs) const
{
	return vec4(x / rhs,
		y / rhs,
		z / rhs,
		w / rhs);
}

vec4& vec4::operator/=(float rhs)
{
	set(x / rhs,
		y / rhs,
		z / rhs,
		w / rhs);

	return (*this);
}

vec4 vec4::operator-(void) const
{
	return vec4(-x, -y, -z, -w);
}

bool vec4::operator==(const vec4& rhs) const
{
	return (float_equal(x, rhs.x) &&
		float_equal(y, rhs.y) &&
		float_equal(z, rhs.z) &&
		float_equal(w, rhs.w));
}

bool vec4::operator!=(const vec4& rhs) const
{
	return !(*this == rhs);
}

float vec4::distance(const vec4& a, const vec4& b)
{
	vec4 diff = b - a;
	return (diff.length());
}

float vec4::distance_sq(const vec4& a, const vec4& b)
{
	vec4 diff = b - a;
	return (diff.length_sq());
}
