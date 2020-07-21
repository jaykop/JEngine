/******************************************************************************/
/*!
\file   input_handler.hpp
\author Jeong Juyong
\par    email: jaykop.jy\@gmail.com
\date   2019/06/11(yy/mm/dd)

\description
Contains the definition of InputHandler class
*/
/******************************************************************************/
#pragma once

#include <set>
#include <key_map.hpp>
#include <unordered_map>
#include <vec3.hpp>

union SDL_Event;

jeBegin

using KeyMap = std::unordered_map<KEY, bool>;
using LockedKeys = std::set<KEY>;

class InputHandler {

	// Prevent to clone this class
	InputHandler() = delete;
	~InputHandler() = delete;

	jePreventClone(InputHandler)

	friend class Application;
	friend class SceneManager;

public:

	enum class MouseWheel { NONE, UP, DOWN } ;

	static bool any_key_down(); // check both mouse or key
	static bool any_mouse_down();
	static bool any_input_down();

	static bool key_pressed(KEY key);
	static bool key_triggered(KEY key);
	static bool get_mouse_wheel_status(KEY key);

	// mouse position
	static vec3 get_position();
	static vec3 get_ray_direction();
	static bool ray_intersects_triangle(const vec3& v0, const vec3& v1, const vec3& v2);

private:

	static KEY key_translator(const SDL_Event& event);
	static KEY mouse_translator(const SDL_Event& event);
	static void mouse_refresh(SDL_Event& event);
	static void lock_triggered_keys();

	static void initialize();
	static void update(const SDL_Event& event);
	static void close();

	static float wheelSensitivity_;
	static bool mouseDown_, keyDown_;
	static KeyMap keyMap_, triggerMap_;
	static MouseWheel mouseWheel_;
	static LockedKeys triggerLock_;

	static vec3 position_;
	
};

jeEnd