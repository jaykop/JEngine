/******************************************************************************/
/*!
\file   macro.hpp
\author Jeong Juyong
\par    email: jaykop.jy\@gmail.com
\date   2019/06/02(yy/mm/dd)

\description
Contains the engine's generic macros
*/
/******************************************************************************/

#pragma once

// namespace
#ifdef __cplusplus
#define jeBegin	namespace JE {
#define jeEnd	}
#endif

// Debug printing
#define jeDoPrint
#if defined(_DEBUG) && defined(jeDoPrint)
#include <cstdio>
#define jeDebugPrint(...) printf(__VA_ARGS__)
#else
#define jeDebugPrint(...) jeUnused(__VA_ARGS__)
#endif 

// Set variable as unreferenced
#define jeUnused(unused) (void)unused

// Prevent to clone this class
#define jePreventClone(x)			\
	x(x&&) = delete;				\
	x(const x&) = delete;			\
	x& operator=(x&&) = delete;		\
	x& operator=(const x&) = delete;	