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
KeyMap InputHandler::keyMap_, InputHandler::triggerMap_;
InputHandler::MouseWheel InputHandler::mouseWheel_ = MouseWheel::NONE;
bool InputHandler::mouseDown_ = false, InputHandler::keyDown_ = false;
LockedKeys InputHandler::triggerLock_;

bool InputHandler::any_input_down()
{
	return mouseDown_ || keyDown_;
}

bool InputHandler::any_key_down()
{
	return keyDown_;
}

bool InputHandler::any_mouse_down()
{
	return mouseDown_;
}

bool InputHandler::key_pressed(KEY key)
{
	return keyMap_[key];
}

bool InputHandler::key_triggered(KEY key)
{
	if (triggerMap_[key])
	{
		triggerLock_.insert(key);
		return true;
	}

	return false;
}

KEY InputHandler::key_translator(const SDL_Event& event)
{
	// Keyboard translator
	switch (event.key.keysym.sym) {

	case SDLK_UNKNOWN:	break;
	case SDLK_RETURN: return KEY::ENTER; 
	case SDLK_ESCAPE: return KEY::ESC; 
	case SDLK_BACKSPACE: return KEY::BACK; 
	case SDLK_TAB: return KEY::TAB;	
	case SDLK_SPACE: return KEY::SPACE;	
	case SDLK_RIGHT: return KEY::RIGHT;	
	case SDLK_LEFT:	return KEY::LEFT; 
	case SDLK_DOWN:	return KEY::DOWN; 
	case SDLK_UP: return KEY::UP; 
	case SDLK_0: return KEY::NUM_0;	
	case SDLK_1: return KEY::NUM_1;	
	case SDLK_2: return KEY::NUM_2;	
	case SDLK_3: return KEY::NUM_3;	
	case SDLK_4: return KEY::NUM_4;	
	case SDLK_5: return KEY::NUM_5;	
	case SDLK_6: return KEY::NUM_6;	
	case SDLK_7: return KEY::NUM_7;	
	case SDLK_8: return KEY::NUM_8;	
	case SDLK_9: return KEY::NUM_9;	
	case SDLK_a: return KEY::A;	
	case SDLK_b: return KEY::B;	
	case SDLK_c: return KEY::C;	
	case SDLK_d: return KEY::D;	
	case SDLK_e: return KEY::E;	
	case SDLK_f: return KEY::F;	
	case SDLK_g: return KEY::G;	
	case SDLK_h: return KEY::H;	
	case SDLK_i: return KEY::I;	
	case SDLK_j: return KEY::J;	
	case SDLK_k: return KEY::K;	
	case SDLK_l: return KEY::L;	
	case SDLK_m: return KEY::M;	
	case SDLK_n: return KEY::N;	
	case SDLK_o: return KEY::O;	
	case SDLK_p: return KEY::P;	
	case SDLK_q: return KEY::Q;	
	case SDLK_r: return KEY::R;	
	case SDLK_s: return KEY::S;	
	case SDLK_t: return KEY::T;	
	case SDLK_u: return KEY::U;	
	case SDLK_v: return KEY::V;	
	case SDLK_w: return KEY::W;	
	case SDLK_x: return KEY::X;	
	case SDLK_y: return KEY::Y;	
	case SDLK_z: return KEY::Z;	
	//case SDLK_CAPSLOCK: return JE_CAPSLOCK; break;
	}

	return KEY::NONE;
}

KEY InputHandler::mouse_translator(const SDL_Event& event)
{
		// Mouse translator
	switch (event.button.button) {

	case SDL_BUTTON_RIGHT: return KEY::MOUSE_RIGHT;
	case SDL_BUTTON_LEFT: return KEY::MOUSE_LEFT;
	case SDL_BUTTON_MIDDLE:	return KEY::MOUSE_MIDDLE;
	}

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
		keyMap_.insert(pair);
		triggerMap_.insert(pair);
	}
}

void InputHandler::update(const SDL_Event& event)
{
	auto key = key_translator(event);
	auto mouse = mouse_translator(event);

	switch (event.type) {
	
	case SDL_KEYUP:
	{
		triggerLock_.clear();
		triggerMap_[key] = keyMap_[key] = false;
		keyDown_ = false;

		break;
	}

	case SDL_KEYDOWN:
	{
		triggerMap_[key] = keyMap_[key] = true;
		keyDown_ = true;

		break;
	}
	
	case SDL_MOUSEBUTTONUP:
	{
		triggerMap_[mouse] = keyMap_[mouse] = false;
		mouseDown_ = false;

		break;
	}

	case SDL_MOUSEBUTTONDOWN:
	{
		triggerMap_[mouse] = keyMap_[mouse] = true;
		mouseDown_ = true;

		break;
	}

	case SDL_MOUSEWHEEL:
	{
		if (event.wheel.y > wheelSensitivity_)
			mouseWheel_ = MouseWheel::UP;

		else if (event.wheel.y < -wheelSensitivity_)
			mouseWheel_ = MouseWheel::DOWN;

		break;
	}
	}

}

void InputHandler::close()
{
	keyMap_.clear();
}

void InputHandler::mouse_refresh(SDL_Event& event)
{
	// set default wheel level
	mouseWheel_ = MouseWheel::NONE;
	event.wheel.y = 0;
}

void InputHandler::lock_triggered_keys()
{
	for (auto& key : triggerLock_)
		triggerMap_[key] = false;
}

bool InputHandler::get_mouse_wheel_status(KEY key)
{
	if ((key == KEY::MOUSE_WHEEL_UP && mouseWheel_ == MouseWheel::UP)
		|| (key == KEY::MOUSE_WHEEL_DOWN && mouseWheel_ == MouseWheel::DOWN))
		return true;
	
	return false;
}

jeEnd
