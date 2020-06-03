#include <mat4.hpp>
#include <vec4.hpp>
#include <math_util.hpp>
#include <algorithm>

using namespace Math;

const mat4 mat4::identity(1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f);

mat4::mat4(void)
	: m00(1.f), m01(0.f), m02(0.f), m03(0.f)
	, m10(0.f), m11(1.f), m12(0.f), m13(0.f)
	, m20(0.f), m21(0.f), m22(1.f), m23(0.f)
	, m30(0.f), m31(0.f), m32(0.f), m33(1.f)
{}

mat4::mat4(float _m00, float _m01, float _m02, float _m03,
	float _m10, float _m11, float _m12, float _m13,
	float _m20, float _m21, float _m22, float _m23,
	float _m30, float _m31, float _m32, float _m33)
	: m00(_m00), m01(_m01), m02(_m02), m03(_m03)
	, m10(_m10), m11(_m11), m12(_m12), m13(_m13)
	, m20(_m20), m21(_m21), m22(_m22), m23(_m23)
	, m30(_m30), m31(_m31), m32(_m32), m33(_m33)
{
}

void mat4::set(float _m00, float _m01, float _m02, float _m03,
	float _m10, float _m11, float _m12, float _m13,
	float _m20, float _m21, float _m22, float _m23,
	float _m30, float _m31, float _m32, float _m33)
{
	m00 = _m00; m01 = _m01; m02 = _m02; m03 = _m03;
	m10 = _m10; m11 = _m11; m12 = _m12; m13 = _m13;
	m20 = _m20; m21 = _m21; m22 = _m22; m23 = _m23;
	m30 = _m30; m31 = _m31; m32 = _m32; m33 = _m33;
}

void mat4::set_identity(void)
{
	set(1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
}

mat4& mat4::invert(void)
{
	// calculate expansions for determinant
	float exp00 = m11 * (m22 * m33 - m32 * m23) -
		m12 * (m21 * m33 - m31 * m23) +
		m13 * (m21 * m32 - m31 * m22);
	float exp01 = -(m10 * (m22 * m33 - m32 * m23) -
		m12 * (m20 * m33 - m30 * m23) +
		m13 * (m20 * m32 - m30 * m22));
	float exp02 = m10 * (m21 * m33 - m31 * m23) -
		m11 * (m20 * m33 - m30 * m23) +
		m13 * (m20 * m31 - m30 * m21);
	float exp03 = -(m10 * (m21 * m32 - m31 * m22) -
		m11 * (m20 * m32 - m30 * m22) +
		m12 * (m20 * m31 - m30 * m21));

	float det = m00 * exp00 + m01 * exp01 + m02 * exp02 + m03 * exp03;

	// calculate adjoint matrix (transposed matrix of cofactors)
	set(exp00,

		-(m01 * (m22 * m33 - m32 * m23) -
			m02 * (m21 * m33 - m31 * m23) +
			m03 * (m21 * m32 - m31 * m22)),

		m01 * (m12 * m33 - m32 * m13) -
		m02 * (m11 * m33 - m31 * m13) +
		m03 * (m11 * m32 - m31 * m12),

		-(m01 * (m12 * m23 - m22 * m13) -
			m02 * (m11 * m23 - m21 * m13) +
			m03 * (m11 * m22 - m21 * m12)),
		///////////////////////////////
		exp01,

		m00 * (m22 * m33 - m32 * m23) -
		m02 * (m20 * m33 - m30 * m23) +
		m03 * (m20 * m32 - m30 * m22),

		-(m00 * (m12 * m33 - m32 * m13) -
			m02 * (m10 * m33 - m30 * m13) +
			m03 * (m10 * m32 - m30 * m12)),

		m00 * (m12 * m23 - m22 * m13) -
		m02 * (m10 * m23 - m20 * m13) +
		m03 * (m10 * m22 - m20 * m12),
		///////////////////////////////
		exp02,

		-(m00 * (m21 * m33 - m31 * m23) -
			m01 * (m20 * m33 - m30 * m23) +
			m03 * (m20 * m31 - m30 * m21)),

		m00 * (m11 * m33 - m31 * m13) -
		m01 * (m10 * m33 - m30 * m13) +
		m03 * (m10 * m31 - m30 * m11),

		-(m00 * (m11 * m23 - m21 * m13) -
			m01 * (m10 * m23 - m20 * m13) +
			m03 * (m10 * m21 - m20 * m11)),
		///////////////////////////////
		exp03,

		m00 * (m21 * m32 - m31 * m22) -
		m01 * (m20 * m32 - m30 * m22) +
		m02 * (m20 * m31 - m30 * m21),

		-(m00 * (m11 * m32 - m31 * m12) -
			m01 * (m10 * m32 - m30 * m12) +
			m02 * (m10 * m31 - m30 * m11)),

		m00 * (m11 * m22 - m21 * m12) -
		m01 * (m10 * m22 - m20 * m12) +
		m02 * (m10 * m21 - m20 * m11));

	*this *= (1.0f / det);
	return (*this);
}

mat4 mat4::inverted(void) const
{
	mat4 invert(*this);
	invert.invert();

	return invert;
}

mat4& mat4::transpose(void)
{
	set(m00, m10, m20, m30,
		m01, m11, m21, m31,
		m02, m12, m22, m32,
		m03, m13, m23, m33);

	return (*this);
}

mat4 mat4::transposed(void) const
{
	return mat4(m00, m10, m20, m30,
		m01, m11, m21, m31,
		m02, m12, m22, m32,
		m03, m13, m23, m33);
}

mat3 mat4::to_mat3(void) const
{
	return mat3(m00, m01, m02,
		m10, m11, m12,
		m20, m21, m22);
}

float mat4::determinant(void) const
{
	return (m00 * (m11 * (m22 * m33 - m32 * m23) -
		m12 * (m21 * m33 - m31 * m23) +
		m13 * (m21 * m32 - m31 * m22)) -

		m01 * (m10 * (m22 * m33 - m32 * m23) -
			m12 * (m20 * m33 - m30 * m23) +
			m13 * (m20 * m32 - m30 * m22)) +

		m02 * (m10 * (m21 * m33 - m31 * m23) -
			m11 * (m20 * m33 - m30 * m23) +
			m13 * (m20 * m31 - m30 * m21)) -

		m03 * (m10 * (m21 * m32 - m31 * m22) -
			m11 * (m20 * m32 - m30 * m22) +
			m12 * (m20 * m31 - m30 * m21)));
}

mat4 mat4::operator+(const mat4& rhs) const
{
	return mat4(m00 + rhs.m00, m01 + rhs.m01, m02 + rhs.m02, m03 + rhs.m03,
		m10 + rhs.m10, m11 + rhs.m11, m12 + rhs.m12, m13 + rhs.m13,
		m20 + rhs.m20, m21 + rhs.m21, m22 + rhs.m22, m23 + rhs.m23,
		m30 + rhs.m30, m31 + rhs.m31, m32 + rhs.m32, m33 + rhs.m33);
}

mat4& mat4::operator+=(const mat4& rhs)
{
	set(m00 + rhs.m00, m01 + rhs.m01, m02 + rhs.m02, m03 + rhs.m03,
		m10 + rhs.m10, m11 + rhs.m11, m12 + rhs.m12, m13 + rhs.m13,
		m20 + rhs.m20, m21 + rhs.m21, m22 + rhs.m22, m23 + rhs.m23,
		m30 + rhs.m30, m31 + rhs.m31, m32 + rhs.m32, m33 + rhs.m33);

	return (*this);
}

mat4 mat4::operator-(const mat4& rhs) const
{
	return mat4(m00 - rhs.m00, m01 - rhs.m01, m02 - rhs.m02, m03 - rhs.m03,
		m10 - rhs.m10, m11 - rhs.m11, m12 - rhs.m12, m13 - rhs.m13,
		m20 - rhs.m20, m21 - rhs.m21, m22 - rhs.m22, m23 - rhs.m23,
		m30 - rhs.m30, m31 - rhs.m31, m32 - rhs.m32, m33 - rhs.m33);
}

mat4& mat4::operator-=(const mat4& rhs)
{
	set(m00 - rhs.m00, m01 - rhs.m01, m02 - rhs.m02, m03 - rhs.m03,
		m10 - rhs.m10, m11 - rhs.m11, m12 - rhs.m12, m13 - rhs.m13,
		m20 - rhs.m20, m21 - rhs.m21, m22 - rhs.m22, m23 - rhs.m23,
		m30 - rhs.m30, m31 - rhs.m31, m32 - rhs.m32, m33 - rhs.m33);

	return (*this);
}

mat4 mat4::operator*(const mat4& rhs) const
{
	return mat4(m00 * rhs.m00 + m01 * rhs.m10 + m02 * rhs.m20 + m03 * rhs.m30,
		m00 * rhs.m01 + m01 * rhs.m11 + m02 * rhs.m21 + m03 * rhs.m31,
		m00 * rhs.m02 + m01 * rhs.m12 + m02 * rhs.m22 + m03 * rhs.m32,
		m00 * rhs.m03 + m01 * rhs.m13 + m02 * rhs.m23 + m03 * rhs.m33,

		m10 * rhs.m00 + m11 * rhs.m10 + m12 * rhs.m20 + m13 * rhs.m30,
		m10 * rhs.m01 + m11 * rhs.m11 + m12 * rhs.m21 + m13 * rhs.m31,
		m10 * rhs.m02 + m11 * rhs.m12 + m12 * rhs.m22 + m13 * rhs.m32,
		m10 * rhs.m03 + m11 * rhs.m13 + m12 * rhs.m23 + m13 * rhs.m33,

		m20 * rhs.m00 + m21 * rhs.m10 + m22 * rhs.m20 + m23 * rhs.m30,
		m20 * rhs.m01 + m21 * rhs.m11 + m22 * rhs.m21 + m23 * rhs.m31,
		m20 * rhs.m02 + m21 * rhs.m12 + m22 * rhs.m22 + m23 * rhs.m32,
		m20 * rhs.m03 + m21 * rhs.m13 + m22 * rhs.m23 + m23 * rhs.m33,

		m30 * rhs.m00 + m31 * rhs.m10 + m32 * rhs.m20 + m33 * rhs.m30,
		m30 * rhs.m01 + m31 * rhs.m11 + m32 * rhs.m21 + m33 * rhs.m31,
		m30 * rhs.m02 + m31 * rhs.m12 + m32 * rhs.m22 + m33 * rhs.m32,
		m30 * rhs.m03 + m31 * rhs.m13 + m32 * rhs.m23 + m33 * rhs.m33);
}

mat4& mat4::operator*=(const mat4& rhs)
{
	set(m00 * rhs.m00 + m01 * rhs.m10 + m02 * rhs.m20 + m03 * rhs.m30,
		m00 * rhs.m01 + m01 * rhs.m11 + m02 * rhs.m21 + m03 * rhs.m31,
		m00 * rhs.m02 + m01 * rhs.m12 + m02 * rhs.m22 + m03 * rhs.m32,
		m00 * rhs.m03 + m01 * rhs.m13 + m02 * rhs.m23 + m03 * rhs.m33,

		m10 * rhs.m00 + m11 * rhs.m10 + m12 * rhs.m20 + m13 * rhs.m30,
		m10 * rhs.m01 + m11 * rhs.m11 + m12 * rhs.m21 + m13 * rhs.m31,
		m10 * rhs.m02 + m11 * rhs.m12 + m12 * rhs.m22 + m13 * rhs.m32,
		m10 * rhs.m03 + m11 * rhs.m13 + m12 * rhs.m23 + m13 * rhs.m33,

		m20 * rhs.m00 + m21 * rhs.m10 + m22 * rhs.m20 + m23 * rhs.m30,
		m20 * rhs.m01 + m21 * rhs.m11 + m22 * rhs.m21 + m23 * rhs.m31,
		m20 * rhs.m02 + m21 * rhs.m12 + m22 * rhs.m22 + m23 * rhs.m32,
		m20 * rhs.m03 + m21 * rhs.m13 + m22 * rhs.m23 + m23 * rhs.m33,

		m30 * rhs.m00 + m31 * rhs.m10 + m32 * rhs.m20 + m33 * rhs.m30,
		m30 * rhs.m01 + m31 * rhs.m11 + m32 * rhs.m21 + m33 * rhs.m31,
		m30 * rhs.m02 + m31 * rhs.m12 + m32 * rhs.m22 + m33 * rhs.m32,
		m30 * rhs.m03 + m31 * rhs.m13 + m32 * rhs.m23 + m33 * rhs.m33);

	return (*this);
}

vec4 mat4::operator*(const vec4& rhs) const
{
	return vec4(m00 * rhs.x + m01 * rhs.y + m02 * rhs.z + m03 * rhs.w,
		m10 * rhs.x + m11 * rhs.y + m12 * rhs.z + m13 * rhs.w,
		m20 * rhs.x + m21 * rhs.y + m22 * rhs.z + m23 * rhs.w,
		m30 * rhs.x + m31 * rhs.y + m32 * rhs.z + m33 * rhs.w);
}

mat4 mat4::operator*(float rhs) const
{
	return mat4(m00 * rhs, m01 * rhs, m02 * rhs, m03 * rhs,
		m10 * rhs, m11 * rhs, m12 * rhs, m13 * rhs,
		m20 * rhs, m21 * rhs, m22 * rhs, m23 * rhs,
		m30 * rhs, m31 * rhs, m32 * rhs, m33 * rhs);
}

mat4& mat4::operator*=(float rhs)
{
	set(m00 * rhs, m01 * rhs, m02 * rhs, m03 * rhs,
		m10 * rhs, m11 * rhs, m12 * rhs, m13 * rhs,
		m20 * rhs, m21 * rhs, m22 * rhs, m23 * rhs,
		m30 * rhs, m31 * rhs, m32 * rhs, m33 * rhs);

	return (*this);
}

mat4 mat4::operator/(float rhs) const
{
	return mat4(m00 / rhs, m01 / rhs, m02 / rhs, m03 / rhs,
		m10 / rhs, m11 / rhs, m12 / rhs, m13 / rhs,
		m20 / rhs, m21 / rhs, m22 / rhs, m23 / rhs,
		m30 / rhs, m31 / rhs, m32 / rhs, m33 / rhs);
}

mat4& mat4::operator/=(float rhs)
{
	set(m00 / rhs, m01 / rhs, m02 / rhs, m03 / rhs,
		m10 / rhs, m11 / rhs, m12 / rhs, m13 / rhs,
		m20 / rhs, m21 / rhs, m22 / rhs, m23 / rhs,
		m30 / rhs, m31 / rhs, m32 / rhs, m33 / rhs);

	return (*this);
}

bool mat4::operator==(const mat4& rhs) const
{
	return (float_equal(m00, rhs.m00) &&
		float_equal(m01, rhs.m01) &&
		float_equal(m02, rhs.m02) &&
		float_equal(m03, rhs.m03) &&
		float_equal(m10, rhs.m10) &&
		float_equal(m11, rhs.m11) &&
		float_equal(m12, rhs.m12) &&
		float_equal(m13, rhs.m13) &&
		float_equal(m20, rhs.m20) &&
		float_equal(m21, rhs.m21) &&
		float_equal(m22, rhs.m22) &&
		float_equal(m23, rhs.m23) &&
		float_equal(m30, rhs.m30) &&
		float_equal(m31, rhs.m31) &&
		float_equal(m32, rhs.m32) &&
		float_equal(m33, rhs.m33));
}

bool mat4::operator!=(const mat4& rhs) const
{
	return !(*this == rhs);
}
