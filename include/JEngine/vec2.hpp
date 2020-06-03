#pragma once
#pragma warning(push)
// disable nameless struct/union warning
#pragma warning(disable : 4201)

  /* 2 dimensional vector of floats */
struct vec2
{
	// static members
	static const vec2 zero;

	// public members
	union
	{
		struct
		{
			float x;
			float y;
		};

		float data[2];
	};

	// ctor / dtor
	vec2(void);
	vec2(float x, float y);

	// public methods
	void set(float x, float y);
	void set(float f);
	void set_zero(void);
	void set_one(void);
	float length(void) const;
	float length_sq(void) const;
	vec2& normalize(void);
	vec2 normalized(void) const;

	// operators
	vec2 operator+(const vec2& rhs) const;
	vec2& operator+=(const vec2& rhs);
	vec2 operator-(const vec2& rhs) const;
	vec2& operator-=(const vec2& rhs);

	vec2 operator*(float rhs) const;
	vec2& operator*=(float rhs);
	vec2 operator/(float rhs) const;
	vec2& operator/=(float rhs);

	vec2 operator-(void) const;

	bool operator==(const vec2& rhs) const;
	bool operator!=(const vec2& rhs) const;

}; // struct vec2

#pragma warning(pop)
