#pragma once
#pragma warning(push)
// disable nameless struct/union warning
#pragma warning(disable : 4201)

#include <vec3.hpp>

struct vec4
{
	// static members
	static const vec4 zero;

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
			float r;
			float g;
			float b;
			float a;
		};

		float data[4];
	};

	// ctors
	vec4(void);
	vec4(float x, float y, float z, float w);
	vec4(const vec3& v, float w);

	// public methods
	void set(float x, float y, float z, float w);
	void set(float f);
	void set_zero(void);
	void set_one(void);
	float length(void) const;
	float length_sq(void) const;
	vec4& normalize(void);
	vec4 normalized(void) const;
	vec3 to_vec3(void) const;

	float dot(const vec4& rhs) const;
	vec4 cross(const vec4& rhs) const;

	// operators
	vec4 operator+(const vec4& rhs) const;
	vec4& operator+=(const vec4& rhs);
	vec4 operator-(const vec4& rhs) const;
	vec4& operator-=(const vec4& rhs);

	vec4 operator*(float rhs) const;
	vec4& operator*=(float rhs);
	vec4 operator/(float rhs) const;
	vec4& operator/=(float rhs);

	vec4 operator-(void) const;

	bool operator==(const vec4& rhs) const;
	bool operator!=(const vec4& rhs) const;

	// static methods
	static float distance(const vec4& a, const vec4& b);
	static float distance_sq(const vec4& a, const vec4& b);

}; // struct vec4

#pragma warning(pop)
