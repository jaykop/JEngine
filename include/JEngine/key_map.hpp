/******************************************************************************/
/*!
\file   key_map.hpp
\author Jeong Juyong
\par    email: jaykop.jy\@gmail.com
\date   2019/06/11(yy/mm/dd)

\description
Contains the key ported from sfml
*/
/******************************************************************************/

#pragma once
#include <macros.hpp>

jeBegin

enum KEY {

	// default
	NONE = -1, 
	
	// alphabet
	A = 0, B, C, D, E, F, G, H, I, J, K,
	L, M, N, O, P, Q, R, S, T, U, V,
	W, X, Y, Z,

	// numbers
	NUM_0, NUM_1, NUM_2, NUM_3, NUM_4, NUM_5, NUM_6, NUM_7, NUM_8, NUM_9,

	// function keys
	ESC = 36, 
	LCONTROL = 37, LSHIFT, LALT, 
	RCONTROL = 41, RSHIFT, RALT, 
	SPACE = 57, ENTER, BACK, TAB, 

	// arrows
	LEFT = 71, RIGHT, UP, DOWN, 
	
	// mouse 
	MOUSE_LEFT, MOUSE_RIGHT, MOUSE_MIDDLE,
	MOUSE_WHEEL_UP, MOUSE_WHEEL_DOWN,

	END
};

jeEnd