#pragma once
#pragma warning(push)
// disable nameless struct/union warning
#pragma warning(disable : 4201)

/* 3 dimensional vector of floats */
struct mat3;
struct vec3
{
	// static members
	static const vec3 zero;
	static const vec3 one;

	// public members
	union
	{
		struct
		{
			float x;
			float y;
			float z;
		};

		struct
		{
			float r;
			float g;
			float b;
		};

		float data[3];
	};

	// ctor / dtor
	vec3(void);
	vec3(float x, float y, float z);
	vec3(float f);
	vec3(const vec3& v3);

	// public methods
	void set(float x, float y, float z);
	void set(const vec3& v3);
	void set(float f);
	void set_zero(void);
	void set_one(void);
	float length(void) const;
	float length_sq(void) const;
	vec3& normalize(void);
	vec3 normalized(void) const;

	float dot(const vec3& rhs) const;
	vec3 cross(const vec3& rhs) const;
	bool inside(const vec3& v1, const vec3& v2, const vec3& v3) const;

	// operators
	vec3 operator+(const vec3& rhs) const;
	vec3 operator*(const vec3 & rhs) const;
	vec3& operator+=(const vec3& rhs);
	vec3 operator-(const vec3& rhs) const;
	vec3& operator-=(const vec3& rhs);

	vec3 operator*(float rhs) const;
	vec3& operator*=(float rhs);
	vec3 operator/(float rhs) const;
	vec3& operator/=(float rhs);

	vec3 operator-(void) const;

	bool operator==(const vec3& rhs) const;
	bool operator!=(const vec3& rhs) const;

	// static methods
	static vec3 abs(const vec3& v);
	static float distance(const vec3& a, const vec3& b);
	static float distance_sq(const vec3& a, const vec3& b);
	static vec3 cross(const vec3& lhs, const vec3& rhs) ;
	static float sign(const vec3& p1, const vec3& p2, const vec3& p3);
	static vec3 rotate(const vec3& p, const vec3& o, float rad);

}; // struct vec3

#pragma warning(pop)
