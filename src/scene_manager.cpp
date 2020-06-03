/******************************************************************************/
/*!
\file   scene_manager.cpp
\author Jeong Juyong
\par    email: jaykop.jy\@gmail.com
\date   2019/06/02(yy/mm/dd)

\description
Contains the methods of SceneManager class
*/
/******************************************************************************/

#include <scene_manager.hpp>
#include <timer.hpp>
#include <scene.hpp>
#include <input_handler.hpp>
#include <gl_manager.hpp>
#include <SDL.h>

jeBegin

// constant value
const float SECOND = 1.f, MAX_FRAME_TIME = 0.25f;

// Initialize the static variables
Timer SceneManager::timer_;
int SceneManager::frames_ = 0;
float SceneManager::frameTime_ = 0.f;
SDL_Window* SceneManager::window_ = nullptr;
Scene* SceneManager::currentScene_ = nullptr;
SceneManager::Status SceneManager::status_ = SceneManager::Status::CHANGE;

bool SceneManager::initialize(SDL_Window* window)
{
	if (window) {

		// get window
		window_ = window;
	
		return true;
	}

	return false;
}

void SceneManager::update(SDL_Event* event)
{
	static float timeStack, elapsedTime, currentTime;
	timeStack = elapsedTime = currentTime = 0.f;

	timer_.start();
	change_scene();

	while (status_ == Status::NONE) // state updating loop
	{
		if (SDL_PollEvent(event))
		{
			InputHandler::update(*event);
			GLManager::update(window_, *event);
		}

		elapsedTime = timer_.get_elapsed_time(); // get elapsed time
		frameTime_ = elapsedTime - currentTime; // get frame time

		// Manually block the rfame skipping
		if (frameTime_ > MAX_FRAME_TIME)
			frameTime_ = MAX_FRAME_TIME;

		timeStack += frameTime_; // stack timer
		frames_++; // stack frames

		// Update the scene and systems
		if (timeStack >= SECOND) {

			currentTime = elapsedTime; // refresh the current time
			currentScene_->update(frameTime_); // update the current scene
			SDL_GL_SwapWindow(window_);

			InputHandler::mouse_refresh(*event); // refresh mouse wheel status

			frames_ = 0;
			timeStack = 0.f;
		}
	}

	switch (status_) {

		// Pause process
	case Status::PAUSE:
		/*SYSTEM::Pause();*/
		break;

		// The case to quit app
	case Status::QUIT:
		while (currentScene_) {
			Scene* last = currentScene_->lastScene_;
			currentScene_->close();
			currentScene_->unload();
			currentScene_ = last;
		}
		break;

		// The case to resume to last state
	case Status::RESUME:
		/*currentScene_->close();
		currentScene_->unload();
		SYSTEM::Resume();*/
		break;

		// The case to restart the current state
		// The case to change to next state
		// The case to resume and change
	case Status::RESTART:
	case Status::CHANGE:
	case Status::RESUME_AND_CHANGE:
		currentScene_->close();
		currentScene_->unload();
		break;

		// Keep updaring - this should not happen
	case Status::NONE:
	default:
		break;
	}
}

void SceneManager::close()
{
	clear_scenes();
}

jeEnd