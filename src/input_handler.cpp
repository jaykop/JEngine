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

#include <camera.hpp>
#include <application.hpp>
#include <graphic_system.hpp>
#include <mat4.hpp>

jeBegin

float InputHandler::wheelSensitivity_ = 0.f;
bool InputHandler::mouseDown_ = false, InputHandler::keyDown_ = false;
vec3 InputHandler::position_ = vec3::one;
InputHandler::MouseWheel InputHandler::mouseWheel_ = MouseWheel::NONE;
LockedKeys InputHandler::triggerLock_;
KeyMap InputHandler::keyMap_, InputHandler::triggerMap_;

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
	case SDLK_CAPSLOCK: return KEY::CAPSLOCK; break;
	case SDLK_F1: return KEY::F1; break;
	case SDLK_F2: return KEY::F2; break;
	case SDLK_F3: return KEY::F3; break;
	case SDLK_F4: return KEY::F4; break;
	case SDLK_F5: return KEY::F5; break;
	case SDLK_F6: return KEY::F6; break;
	case SDLK_F7: return KEY::F7; break;
	case SDLK_F8: return KEY::F8; break;
	case SDLK_F9: return KEY::F9; break;
	case SDLK_F10: return KEY::F10; break;
	case SDLK_F11: return KEY::F11; break;
	case SDLK_F12: return KEY::F12; break;
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
	int x, y;
	SDL_GetMouseState(&x, &y);
	position_.set(static_cast<float>(x), static_cast<float>(y), 0.f);

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
		triggerLock_.clear();
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

	case SDL_MOUSEMOTION:

		if (Application::cursor_hidden())
		{
			bool isFullscreen = Application::get_appdata().isFullscreen;

			float top = 0;
			float right = 0;

			if (isFullscreen)
			{
				right = static_cast<float>(Application::get_appdata().displayWidth);
				top = static_cast<float>(Application::get_appdata().displayHeight);
			}
			else
			{
				right = static_cast<float>(Application::get_appdata().width);
				top = static_cast<float>(Application::get_appdata().height);
			}

			if (position_.x > right- 2)
				position_.x = 0;
			else if (position_.x < 1)
				position_.x = right;

			if (position_.y > top - 2)
				position_.y = 0;
			else if (position_.y < 1)
				position_.y = top;

			SDL_WarpMouseInWindow(Application::get_window(),
				static_cast<int>(position_.x),
				static_cast<int>(position_.y));
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

vec3 InputHandler::get_position()
{
	return position_;
}

vec3 InputHandler::get_ray_direction()
{
	float x = (2.0f * position_.x) / GraphicSystem::get_width() - 1.0f;
	float y = 1.0f - (2.0f * position_.y) / GraphicSystem::get_height();
	float z = 1.0f;
	vec3 ray_nds = vec3(x, y, z);
	vec4 ray_clip = vec4(x, y, -1.0, 1.0);

	Camera* camera = GraphicSystem::get_camera();
	mat4 viewport = mat4::look_at(camera->position, camera->position + camera->front_, camera->up_);
	mat4 perspective = mat4::perspective(
		camera->fovy_ + camera->zoom, camera->aspect_,
		camera->near_, camera->far_);

	vec4 ray_eye = perspective.inverted() * ray_clip;
	ray_eye = vec4(ray_eye.x, ray_eye.y, -1.f, 0.f);

	vec3 ray_wor = (viewport.inverted() * ray_eye).to_vec3();
	// don't forget to normalise the vector at some point
	ray_wor.normalize();

	return ray_wor;
}

bool InputHandler::ray_intersects_triangle(const vec3& v0, const vec3& v1, const vec3& v2) {

	Camera* camera = GraphicSystem::get_camera();
	const vec3& d = get_ray_direction();
	const vec3& p = camera->position;

	vec3 e1, e2, h, s, q;
	float a, f, u, v;

	e1.set(v1.x - v0.x, v1.y - v0.y, v1.z - v0.z);
	e2.set(v2.x - v0.x, v2.y - v0.y, v2.z - v0.z);

	h = vec3::cross(d, e2);
	a = e1.dot(h);

	if (a > -0.00001 && a < 0.00001)
		return false;

	f = 1 / a;
	s.set(p.x - v0.x, p.y - v0.y, p.z - v0.z);
	u = f * (s.dot(h));

	if (u < 0.0 || u > 1.0)
		return false;

	q = vec3::cross(s, e1);
	v = f * d.dot(q);

	if (v < 0.0 || u + v > 1.0)
		return false;

	// at this stage we can compute t to find out where
	// the intersection point is on the line
	float t = f * e2.dot(q);

	if (t > 0.00001) // ray intersection
		return true;

	else // this means that there is a line intersection
		 // but not a ray intersection
		return false;
}

jeEnd
