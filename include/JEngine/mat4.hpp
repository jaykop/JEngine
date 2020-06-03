#pragma once
#pragma warning(push)
// disable nameless struct/union warning
#pragma warning(disable : 4201)

#include <mat3.hpp>

  // forward declarations
struct vec4;

/* 4x4 matrix of floats */
struct mat4
{
	// static members
	static const mat4 identity;

	// public members
	union
	{
		struct
		{
			float m00, m01, m02, m03;
			float m10, m11, m12, m13;
			float m20, m21, m22, m23;
			float m30, m31, m32, m33;
		};

		float m[4][4];
		float data[16];
	};

	// ctors
	mat4(void);
	mat4(float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23,
		float m30, float m31, float m32, float m33);

	// public methods
	void set(float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23,
		float m30, float m31, float m32, float m33);
	void set_identity(void);

	mat4& invert(void);
	mat4 inverted(void) const;

	mat4& transpose(void);
	mat4 transposed(void) const;

	mat3 to_mat3(void) const;

	float determinant(void) const;

	// operators
	mat4 operator+(const mat4& rhs) const;
	mat4& operator+=(const mat4& rhs);
	mat4 operator-(const mat4& rhs) const;
	mat4& operator-=(const mat4& rhs);

	mat4 operator*(const mat4& rhs) const;
	mat4& operator*=(const mat4& rhs);

	vec4 operator*(const vec4& rhs) const;

	mat4 operator*(float rhs) const;
	mat4& operator*=(float rhs);
	mat4 operator/(float rhs) const;
	mat4& operator/=(float rhs);

	bool operator==(const mat4& rhs) const;
	bool operator!=(const mat4& rhs) const;

	static mat4 translate(const vec3& vec);
	static mat4 scale(const vec3& vec);
	static mat4 rotate_x(float radian);
	static mat4 rotate_y(float radian);
	static mat4 rotate_z(float radian);
	static mat4 rotate(float radian, const vec3& vec);
	static mat4 perspective(float fovy, float aspect, float zNear, float zFar);
	static mat4 look_at(const vec3& eye, const vec3& target, const vec3& up);
	static mat4 orthogonal(float left, float right, float bottom, float top);
	static mat4 orthogonal(float left, float right, float bottom, float top, float zNear, float zFar);


}; // struct mat4

#pragma warning(pop)
