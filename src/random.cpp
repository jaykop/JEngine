/******************************************************************************/
/*!
\file   random.cpp
\author Jeong Juyong
\par    email: jaykop.jy\@gmail.com
\date   2019/06/03(yy/mm/dd)

\description
Contains the methods of random class
*/
/******************************************************************************/

#include <chrono>
#include <random.hpp>
#include <debug_tools.hpp>

jeBegin

std::mt19937 Random::randObj_;

void Random::seed()
{
	// default boolean
	static bool planted = false;
	DEBUG_ASSERT(!planted, "Random seed has been set already!\n");

	// seed only once
	if (!planted) {
		auto currentTime = std::chrono::system_clock::now();
		auto duration = currentTime.time_since_epoch();
		unsigned milliseconds = static_cast<unsigned>(std::chrono::duration_cast<std::chrono::milliseconds>(duration).count());
		randObj_ = std::mt19937(milliseconds);
		planted = true;
	}
}

int Random::get_rand_int(int min_i, int max_i)
{
	DEBUG_ASSERT(min_i <= max_i, "Wrong min and max values\n");
	std::uniform_int_distribution<int> 	intRand(min_i, max_i);
	return intRand(randObj_);
}

bool Random::get_rand_bool(float probabilityOfTrue)
{
	std::bernoulli_distribution boolRand(probabilityOfTrue);
	return boolRand(randObj_);
}

float Random::get_rand_float(float min_f, float max_f)
{
	DEBUG_ASSERT(min_f <= max_f, "Wrong min and max values\n");
	std::uniform_real_distribution<float> floatRand(min_f, max_f);
	return floatRand(randObj_);
}

vec3 Random::get_rand_vec3(float minX, float minY, float minZ, 
	float maxX, float maxY, float maxZ)
{
	return vec3(get_rand_float(minX, maxX),
		get_rand_float(minY, maxY),
		get_rand_float(minZ, maxZ));
}

vec3 Random::get_rand_vec3(const vec3& min, const vec3& max)
{
	return vec3(get_rand_float(min.x, max.x),
		get_rand_float(min.y, max.y),
		get_rand_float(min.z, max.z));
}

vec4 Random::get_rand_vec4(const vec4& min, const vec4& max)
{
	return vec4(get_rand_float(min.x, max.x),
		get_rand_float(min.y, max.y),
		get_rand_float(min.z, max.z),
		get_rand_float(min.w, max.w));
}

vec4 Random::get_rand_vec4(float minX, float minY, float minZ, float minW, 
	float maxX, float maxY, float maxZ, float maxW)
{
	return vec4(get_rand_float(minX, maxX),
		get_rand_float(minY, maxY),
		get_rand_float(minZ, maxZ),
		get_rand_float(minW, maxW));
}


jeEnd
