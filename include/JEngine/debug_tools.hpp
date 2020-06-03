/******************************************************************************/
/*!
\file   debug_tools.hpp
\author Jeong Juyong
\par    email: jaykop.jy\@gmail.com
\date   2019/06/02(yy/mm/dd)

\description
Contains the definition of the debug console
*/
/******************************************************************************/

#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <cstdio>
#include <crtdbg.h>

#pragma comment(lib,"user32.lib")

namespace DebugTools
{
	//These functios should NOT be called
	bool debug_assert(int expression, const char* outputMessage,
		const char* functionName, const char* fileName,
		unsigned lineNumber);

	void create_console(void);
	void destroy_console(void);
	void clear_screen(void);

	// This should be called for IMPORTANT MESSAGES to the user
	void customized_msg_box(const char* msg);
}

//Macro for debug configuration only
#if defined (_DEBUG)

//Use this macro instead of the function to ASSERT in debug only
#define DEBUG_ASSERT(exp, srt) if(DebugTools::debug_assert((exp), (srt),	\
	__FUNCTION__, __FILE__, __LINE__))								\
{																	\
	DebugBreak();													\
}
//Use this macro to create a console in debug only
#define DEBUG_CREATE_CONSOLE() DebugTools::create_console()
//Use this macro to destroy a console in debug only
#define DEBUG_DESTROY_CONSOLE() DebugTools::destroy_console();
//Use this amscro to print to the console in debug only
#define DEBUG_PRINT(...) printf(__VA_ARGS__)
//Use this macro instead of the funciton to clea the console in debug only
#define DEBUG_CLEAR() DebugTools::clear_screen()
//If you have a leak, there is a number in curly braces next to the error,
//Put that numver in this function and check the call stack to see when and
//where the allocation happened. Set it to -1 to have it not break
#define DEBUG_LEAK_CHECKS(x)										\
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);	\
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);				\
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);				\
	_CrtSetBreakAlloc((x));

#define DEBUG_CALL_CHECK()			\
	static int functionCounter = 0; \
	++functionCounter;				\
	DEBUG_ASSERT(functionCounter == 1, "This function should be called only once");

#else
//Debug Macros do nothing in release mode
//Use this macro instead of the function to ASSERT in debug only
#define DEBUG_ASSERT(expression, outputMessage)
#define DEBUG_CREATE_CONSOLE()
#define DEBUG_DESTROY_CONSOLE()
#define DEBUG_PRINT(...)
#define DEBUG_CLEAR()
#define DEBUG_LEAK_CHECKS(x)
#define DEBUG_CALL_CHECK()

#endif