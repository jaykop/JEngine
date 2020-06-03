#pragma once
#pragma warning(push)
// disable nameless struct/union warning
#pragma warning(disable : 4201)

#include <vector>

// forward declarations
struct vec3;

/* 3x3 matrix of floats */
struct mat3
{
	// static members
	static const mat3 identity;

	// public members
	union
	{
		struct
		{
			float m00, m01, m02;
			float m10, m11, m12;
			float m20, m21, m22;
		};

		float m[3][3];
		float data[9];
	};

	// ctors
	mat3(void);
	mat3(float m00, float m01, float m02,
		float m10, float m11, float m12,
		float m20, float m21, float m22);

	// public methods
	void set(float m00, float m01, float m02,
		float m10, float m11, float m12,
		float m20, float m21, float m22);
	void set_identity(void);

	mat3& invert(void);
	mat3 inverted(void) const;

	mat3& transpose(void);
	mat3 transposed(void) const;

	mat3& covariance(const std::vector<vec3>& vertices);

	float determinant(void) const;

	// operators
	mat3 operator+(const mat3& rhs) const;
	mat3& operator+=(const mat3& rhs);
	mat3 operator-(const mat3& rhs) const;
	mat3& operator-=(const mat3& rhs);

	mat3 operator*(const mat3& rhs) const;
	mat3& operator*=(const mat3& rhs);

	vec3 operator*(const vec3& rhs) const;

	mat3 operator*(float rhs) const;
	mat3& operator*=(float rhs);
	mat3 operator/(float rhs) const;
	mat3& operator/=(float rhs);

	bool operator==(const mat3& rhs) const;
	bool operator!=(const mat3& rhs) const;

}; // struct mat3

#pragma warning(pop)