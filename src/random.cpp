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
	DEBUG_ASSERT(!planted, "Random seed has been set already!");

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
	DEBUG_ASSERT(min_i < max_i, "Wrong min and max values");
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
	DEBUG_ASSERT(min_f < max_f, "Wrong min and max values");
	std::uniform_real_distribution<float>	floatRand(min_f, max_f);
	return floatRand(randObj_);
}

jeEnd
