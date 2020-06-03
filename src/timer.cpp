/******************************************************************************/
/*!
\file   Timer.cpp
\author Jeong Juyong
\par    email: jaykop.jy\@gmail.com
\date   2019/06/02(yy/mm/dd)

\description
Contains the methods of Timer class

*/
/******************************************************************************/

#include <ctime>
#include <timer.hpp>

jeBegin

/******************************************************************************/
/*!
\brief - start a time lap
*/
/******************************************************************************/
void Timer::start(void)
{
	time_ = static_cast<float>(clock());
}

/******************************************************************************/
/*!
\brief - get elapsed time
\return - elapsed time since last start function called
*/
/******************************************************************************/
float Timer::get_elapsed_time(void) const
{
	return (static_cast<float>(clock()) - time_) / static_cast<float>(CLOCKS_PER_SEC);
}

/******************************************************************************/
/*!
\brief - get current time info (yy/mm/dd/hh/mm/ss)
\return - Time instance
*/
/******************************************************************************/
Time Timer::get_current_time_info()
{
	time_t theTime = time(nullptr);
	tm timeBuf;
	localtime_s(&timeBuf, &theTime);

	return Time{ timeBuf.tm_year + 1900,
		timeBuf.tm_mon + 1,
		timeBuf.tm_mday,
		timeBuf.tm_hour,
		timeBuf.tm_min,
		timeBuf.tm_sec };
}

jeEnd

