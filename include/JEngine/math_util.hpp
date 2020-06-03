#pragma once
#include <limits>

//! Math Deifnitions
#define jeRounding(x, dig)	( floor((x) * pow(float(10), dig) + 0.5f) / pow(float(10), dig) )

namespace Math
{
	const float zero = 0.F;
	const float one = 1.F;
	const float perpendicular = 90.F;
	const float colinear = 180.F;
	const float round = 360.F;
	const float radian = 0.01745329251994329576923690768489F;
	const float radian_to_degree = 57.2958F;
	const float sqrt_2 = 1.4142135623730950488016887242097f;
	const float pi = 3.1415926535897932384626433832795f;
	const float pi_2 = 1.5707963267948966192313216916397f;
	const float max_float = (std::numeric_limits<float>::max)();
	const float min_float = -max_float;
	const float epsilon = std::numeric_limits<float>::epsilon(); // O.OO1F in some cases

	inline float deg_to_rad(float deg)
	{ 
		return deg * static_cast<float>(radian); 
	}

	inline float rad_to_deg(float rad)
	{ 
		return rad * static_cast<float>(radian_to_degree); 
	}

	inline float get_max(float a, float b)
	{
		if (a > b) return a;
		else return b;
	}

	inline float get_min(float a, float b)
	{
		if (a < b) return a;
		else return b;
	}

	inline bool float_equal(float lhs, float rhs)
	{
		float diff = lhs - rhs;
		return (diff < epsilon && -diff < epsilon);
	}
}