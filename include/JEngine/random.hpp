/******************************************************************************/
/*!
\file   random.hpp
\author Jeong Juyong
\par    email: jaykop.jy\@gmail.com
\date   2019/06/03(yy/mm/dd)

\description
Contains the definition of random class
*/
/******************************************************************************/

#pragma once
#include <macros.hpp>
#include <random>
#include <vec3.hpp>
#include <vec4.hpp>

jeBegin

// random class
class Random {

	// Prevent to clone this class
	Random() = delete;
	~Random() = delete;

	jePreventClone(Random)

	// only application class can seed initially
	friend class Application;

public:

	static int get_rand_int(int min, int max);
	static float get_rand_float(float min, float max);
	static bool get_rand_bool(float probabilityOfTrue);

	static vec3 get_rand_vec3(const vec3& min, const vec3& max);
	static vec3 get_rand_vec3(float minX, float minY, float minZ, 
		float maxX, float maxY, float maxZ);
	static vec4 get_rand_vec4(const vec4& min, const vec4& max);
	static vec4 get_rand_vec4(float minX, float minY, float minZ, float minW,
		float maxX, float maxY, float maxZ, float maxW);

private:

	static void	seed();
	static std::mt19937	randObj_;

};

jeEnd
