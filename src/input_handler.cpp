/******************************************************************************/
/*!
\file   input_handler.cpp
\author Jeong Juyong
\par    email: jaykop.jy\@gmail.com
\date   2019/06/11(yy/mm/dd)

\description
Contains the methods of InputHandler class
*/
/******************************************************************************/

#include <SDL_events.h>
#include <debug_tools.hpp>
#include <input_handler.hpp>
#include <iostream>

jeBegin

float InputHandler::wheelSensitivity_ = 0.f;
KeyMap InputHandler::keyMap, InputHandler::triggerMap;
InputHandler::MouseWheel InputHandler::mouseWheel_ = NONE;
bool InputHandler::mouseDown = false, InputHandler::keyDown = false;

bool InputHandler::any_input_down()
{
	return mouseDown || keyDown;
}

bool InputHandler::any_key_down()
{
	return keyDown;
}

bool InputHandler::any_mouse_down()
{
	return mouseDown;
}

bool InputHandler::key_pressed(KEY key)
{
	return keyMap[key];
}

bool InputHandler::key_triggered(KEY key)
{
	if (keyMap[key]) return !(triggerMap[key]++);
	return false;
}

KEY InputHandler::key_translator(const SDL_Event& event)
{
	// Keyboard translator
	switch (event.key.keysym.sym) {

	case SDLK_UNKNOWN:
		break;

	case SDLK_RETURN:
		return KEY::ENTER;
		break;

	case SDLK_ESCAPE:
		return KEY::ESC;
		break;

	case SDLK_BACKSPACE:
		return KEY::BACK;
		break;

	case SDLK_TAB:
		return KEY::TAB;
		break;

	case SDLK_SPACE:
		return KEY::SPACE;
		break;

	case SDLK_RIGHT:
		return KEY::RIGHT;
		break;

	case SDLK_LEFT:
		return KEY::LEFT;
		break;

	case SDLK_DOWN:
		return KEY::DOWN;
		break;

	case SDLK_UP:
		return KEY::UP;
		break;

	case SDLK_0:
		return KEY::NUM_0;
		break;

	case SDLK_1:
		return KEY::NUM_1;
		break;

	case SDLK_2:
		return KEY::NUM_2;
		break;

	case SDLK_3:
		return KEY::NUM_3;
		break;

	case SDLK_4:
		return KEY::NUM_4;
		break;

	case SDLK_5:
		return KEY::NUM_5;
		break;

	case SDLK_6:
		return KEY::NUM_6;
		break;

	case SDLK_7:
		return KEY::NUM_7;
		break;

	case SDLK_8:
		return KEY::NUM_8;
		break;

	case SDLK_9:
		return KEY::NUM_9;
		break;

	case SDLK_a:
		return KEY::A;
		break;

	case SDLK_b:
		return KEY::B;
		break;

	case SDLK_c:
		return KEY::C;
		break;

	case SDLK_d:
		return KEY::D;
		break;

	case SDLK_e:
		return KEY::E;
		break;

	case SDLK_f:
		return KEY::F;
		break;

	case SDLK_g:
		return KEY::G;
		break;

	case SDLK_h:
		return KEY::H;
		break;

	case SDLK_i:
		return KEY::I;
		break;

	case SDLK_j:
		return KEY::J;
		break;

	case SDLK_k:
		return KEY::K;
		break;

	case SDLK_l:
		return KEY::L;
		break;

	case SDLK_m:
		return KEY::M;
		break;

	case SDLK_n:
		return KEY::N;
		break;

	case SDLK_o:
		return KEY::O;
		break;

	case SDLK_p:
		return KEY::P;
		break;

	case SDLK_q:
		return KEY::Q;
		break;

	case SDLK_r:
		return KEY::R;
		break;

	case SDLK_s:
		return KEY::S;
		break;

	case SDLK_t:
		return KEY::T;
		break;

	case SDLK_u:
		return KEY::U;
		break;

	case SDLK_v:
		return KEY::V;
		break;

	case SDLK_w:
		return KEY::W;
		break;

	case SDLK_x:
		return KEY::X;
		break;

	case SDLK_y:
		return KEY::Y;
		break;

	case SDLK_z:
		return KEY::Z;
		break;

	//case SDLK_CAPSLOCK:
	//	return JE_CAPSLOCK;
	//	break;

	}

	return KEY::NONE;
}

KEY InputHandler::mouse_translator(const SDL_Event& event)
{
		// Mouse translator
	switch (event.button.button) {

	case SDL_BUTTON_RIGHT:
		return KEY::MOUSE_RIGHT;
		break;

	case SDL_BUTTON_LEFT:
		return KEY::MOUSE_LEFT;
		break;

	case SDL_BUTTON_MIDDLE:
		return KEY::MOUSE_MIDDLE;
		break;
	}

	// mouse wheel
	/*witch (event.mouseWheel.delta) {

	default:
		return KEY::NONE;
		break;
	}*/

	// None of key pressed
	return KEY::NONE;
}

void InputHandler::initialize()
{
	// initialize values in the map
	for (int key = KEY::NONE; key <= KEY::END; key++) {
		
		// generate an instance
		KeyMap::value_type pair { KEY(key), false };
		
		// insert to the maps
		keyMap.insert(pair);
		triggerMap.insert(pair);
	}
}

void InputHandler::update(const SDL_Event& event)
{
	switch (event.type) {
	
	case SDL_KEYUP:
	{
		auto key = key_translator(event);
		triggerMap[key] = keyMap[key] = false;
		keyDown = false;

		break;
	}

	case SDL_KEYDOWN:
	{
		keyMap[key_translator(event)] = true;
		keyDown = true;

		break;
	}
	
	case SDL_MOUSEBUTTONUP:
	{
		auto key = mouse_translator(event);
		triggerMap[key] = keyMap[key] = false;
		mouseDown = false;

		break;
	}

	case SDL_MOUSEBUTTONDOWN:
	{
		keyMap[mouse_translator(event)] = true;
		mouseDown = true;

		break;
	}

	case SDL_MOUSEWHEEL:
	{
		if (event.wheel.y > wheelSensitivity_)
			mouseWheel_ = UP;

		else if (event.wheel.y < -wheelSensitivity_)
			mouseWheel_ = DOWN;

		break;
	}
	}
}

void InputHandler::close()
{
	keyMap.clear();
}

void InputHandler::mouse_refresh(SDL_Event& event)
{
	// set default wheel level
	mouseWheel_ = NONE;
	event.wheel.y = 0.f;
}

bool InputHandler::get_mouse_wheel_status(KEY key)
{
	if ((key == KEY::MOUSE_WHEEL_UP && mouseWheel_ == UP)
		|| (key == KEY::MOUSE_WHEEL_DOWN && mouseWheel_ == DOWN))
		return true;
	
	return false;
}

jeEnd
