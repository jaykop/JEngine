/******************************************************************************/
/*!
\file   application.cpp
\author Jeong Juyong
\par    email: jaykop.jy\@gmail.com
\date   2019/06/02(yy/mm/dd)

\description
Contains the definition of Time structure and Timer class

*/
/******************************************************************************/

#pragma once
#include <macros.hpp>

jeBegin

// Generic time structure
struct Time {

	int year, month, day, hour, minute, second;
};

// Generic timer class
class Timer {

public:

	Timer() :time_(0.f) {};
	~Timer() {};

	void	start(void);
	float	get_elapsed_time(void) const;
	static  Time get_current_time_info(void);

private:

	float time_;

	// Prevent to clone this class
	Timer(Timer&&) = delete;
	Timer(const Timer&) = delete;
	Timer& operator=(Timer&&) = delete;
	Timer& operator=(const Timer&) = delete;

};

jeEnd
