#pragma once
#pragma warning(push)
// disable nameless struct/union warning
#pragma warning(disable : 4201)

#include <vec4.hpp>
#include <vec3.hpp>
#include <mat4.hpp>
#include <mat3.hpp>

  /* quaternion for representing orientation */
struct quat
{
	// static members
	static const quat identity;

	// public members
	union
	{
		struct
		{
			float x;
			float y;
			float z;
			float w;
		};

		struct
		{
			vec3 v;
			float s;
		};

		float data[4];
	};

	// ctors
	quat(void);
	quat(float x, float y, float z, float w);
	quat(float radians, const vec4& axis);
	quat(float radians, const vec3& axis);
	quat(const mat4& m);
	quat(const mat3& m);
	quat(const quat& q);

	// public methods
	quat& normalize(void);
	quat normalized(void) const;

	quat& invert(void);
	quat inverted(void) const;

	float dot(const quat& rhs) const;

	void set_explicit(float x, float y, float z, float w);
	void set_explicit(float s, const vec3& v);

	// axis-angle setters
	void set(float angle_rad, float axis_x, float axis_y, float axis_z);
	void set(float angle_rad, const vec4& axis);
	void set(float angle_rad, const vec3& axis);

	// euler
	void set_euler(float rad_x, float rad_y, float rad_z);
	void set_euler(const vec3& rad);
	vec3 get_euler(void) const;

	// matrix setters
	void set(const mat4& m);
	void set(const mat3& m);

	// conversions
	mat4 to_mat4(void) const;
	mat3 to_mat3(void) const;

	void to_matrix(mat4& out_matrix) const;
	void to_matrix(mat3& out_matrix) const;

	// operators
	quat operator*(const quat& rhs) const;
	quat& operator*=(const quat& rhs);

	quat operator-(void) const;

	bool operator==(const quat& rhs) const;
	bool operator!=(const quat& rhs) const;

}; // struct quat

#pragma warning(pop)
