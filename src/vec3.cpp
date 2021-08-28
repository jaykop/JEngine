#include <cmath>
#include <vec3.hpp>
#include <math_util.hpp>
#include <mat3.hpp>

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

vec3::vec3(float f)
	: x(f), y(f), z(f)
{
}

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
	return (x * x + y * y + z * z);
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
	if (rhs.x != x || rhs.y != y || rhs.z != z)
		return false;

	return true;

	/*return (float_equal(x, rhs.x) &&
		float_equal(y, rhs.y) &&
		float_equal(z, rhs.z));*/
}

bool vec3::operator!=(const vec3& rhs) const
{
	return !(*this == rhs);
}

vec3 vec3::operator*(const mat3& rhs) const
{
	return vec3(x*rhs.m00 + y*rhs.m10 + z*rhs.m20,
		x * rhs.m01 + y * rhs.m11 + z * rhs.m21,
		x * rhs.m02 + y * rhs.m12 + z * rhs.m22);
}

vec3& vec3::operator*=(const mat3& rhs)
{
	float vx = x, vy = y, vz = z;
	x = vx * rhs.m00 + vy * rhs.m10 + vz * rhs.m20;
	y = vx * rhs.m01 + vy * rhs.m11 + vz * rhs.m21;
	z = vx * rhs.m02 + vy * rhs.m12 + vz * rhs.m22;
	return *this;
}

vec3 vec3::abs(const vec3& v)
{
	vec3 toReturn = v;
	if (toReturn.x < 0.f) toReturn.x = -toReturn.x;
	if (toReturn.y < 0.f) toReturn.y = -toReturn.y;
	if (toReturn.z < 0.f) toReturn.z = -toReturn.z;

	return toReturn;
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

float vec3::sign(const vec3& p1, const vec3& p2, const vec3& p3)
{ 
	return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y); 
}

vec3 vec3::rotate(const vec3& p, const vec3& o, float rad)
{
	float rx = cos(rad) * (p.x - o.x) - sin(rad) * (p.y-o.y) + o.x;
	float ry = sin(rad) * (p.x - o.x) + cos(rad) * (p.y-o.y) + o.y;

	return vec3(rx, ry, 0.f);
}

bool vec3::inside(const vec3& v1, const vec3& v2, const vec3& v3) const
{ 
	bool b1, b2, b3;  
	b1 = sign(*this, v1, v2) < 0.0f;
	b2 = sign(*this, v2, v3) < 0.0f;
	b3 = sign(*this, v3, v1) < 0.0f;
	return ((b1 == b2) && (b2 == b3)); 
}

std::ostream& operator << (std::ostream& os, const vec3& v)
{
	os << v.x << ", " << v.y << ", " << v.z;
	return os;
}