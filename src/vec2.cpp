#include <cmath>
#include <vec2.hpp>
#include <math_util.hpp>

using namespace Math;

const vec2 vec2::zero = vec2(0.0f, 0.0f);

vec2::vec2(void)
{
}

vec2::vec2(float _x, float _y)
	: x(_x)
	, y(_y)
{
}

void vec2::set(float _x, float _y)
{
	x = _x;
	y = _y;
}

void vec2::set(float f)
{
	x = y = f;
}

void vec2::set_zero(void)
{
	x = y = 0.0f;
}

void vec2::set_one(void)
{
	x = y = 1.0f;
}

float vec2::length(void) const
{
	return (sqrt(length_sq()));
}

float vec2::length_sq(void) const
{
	return (x * x + y * y);
}

vec2& vec2::normalize(void)
{
	float len = length();
	*this /= len;
	return (*this);
}

vec2 vec2::normalized(void) const
{
	float len = length();
	return (*this / len);
}

vec2 vec2::operator+(const vec2& rhs) const
{
	return (vec2(x + rhs.x, y + rhs.y));
}

vec2& vec2::operator+=(const vec2& rhs)
{
	x += rhs.x;
	y += rhs.y;

	return (*this);
}

vec2 vec2::operator-(const vec2& rhs) const
{
	return (vec2(x - rhs.x, y - rhs.y));
}

vec2& vec2::operator-=(const vec2& rhs)
{
	x -= rhs.x;
	y -= rhs.y;

	return (*this);
}

vec2 vec2::operator*(float rhs) const
{
	return (vec2(x * rhs, y * rhs));
}

vec2& vec2::operator*=(float rhs)
{
	x *= rhs;
	y *= rhs;

	return (*this);
}

vec2 vec2::operator/(float rhs) const
{
	return (vec2(x / rhs, y / rhs));
}

vec2& vec2::operator/=(float rhs)
{
	x /= rhs;
	y /= rhs;

	return (*this);
}

vec2 vec2::operator-(void) const
{
	return (vec2(-x, -y));
}

bool vec2::operator==(const vec2& rhs) const
{
	return (float_equal(x, rhs.x) && float_equal(y, rhs.y));
}

bool vec2::operator!=(const vec2& rhs) const
{
	return !(*this == rhs);
}
