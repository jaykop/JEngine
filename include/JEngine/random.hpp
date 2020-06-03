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
//#include <vec3.hpp>
//#include <vec4.hpp>

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

private:

	static void	seed();
	static std::mt19937	randObj_;

};

jeEnd
