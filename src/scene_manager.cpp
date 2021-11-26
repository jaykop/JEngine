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

#include <SDL.h>
#include <scene_manager.hpp>
#include <scene.hpp>
#include <input_handler.hpp>
#include <timer.hpp>
#include <debug_tools.hpp>
#include <application.hpp>

jeBegin

// constant value
const float SECOND = 1.f, MAX_FRAME_TIME = 0.25f;
const float FIXED_FRAME = 120.f, UPDATE = SECOND / FIXED_FRAME;

// Initialize the static variables
Timer SceneManager::timer_;
int SceneManager::frames_ = 0;
float SceneManager::frameTime_ = 0.f;
SDL_Window* SceneManager::window_ = nullptr;
Scene* SceneManager::currentScene_ = nullptr;
SceneManager::Status SceneManager::status_ = SceneManager::Status::RESTART;
SceneMap SceneManager::scenes_;
Scene* SceneManager::nextScene_ = nullptr;

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
	static float timeStack, oldTime, currentTime;
	timeStack = oldTime = currentTime = 0.f;

	timer_.start();
	change_scene();

	while (status_ == Status::NONE) // state updating loop
	{
		if (SDL_PollEvent(event))
		{
			InputHandler::update(*event);
			Application::event_update();
		}

		currentTime = timer_.get_elapsed_time(); // get elapsed time
		frameTime_ = currentTime - oldTime; // get frame time
		oldTime = currentTime;

		// Manually block the rfame skipping
		if (frameTime_ > MAX_FRAME_TIME)
			frameTime_ = MAX_FRAME_TIME;

		timeStack += frameTime_; // stack timer
		frames_++; // stack frames

		// Update the scene and systems
		if (timeStack >= UPDATE) {

			currentScene_->update(timeStack); // update the current scene
			SDL_GL_SwapWindow(window_);

			InputHandler::mouse_refresh(*event); // refresh mouse wheel status

			frames_ = 0;
			timeStack = 0.f;
		}

		InputHandler::lock_triggered_keys();
	}

	warp_up_scene();
}

void SceneManager::close()
{
	clear_scenes();
}

void SceneManager::change_scene()
{
	switch (status_)
	{
	case Status::CHANGE:
	{
		delete currentScene_;
		currentScene_ = nextScene_;
		currentScene_->load();
		currentScene_->initialize();
		status_ = Status::NONE;
		break;
	}
	case Status::PAUSE:
	{
		// save the scene to retusm
		Scene* toResume = currentScene_;
		currentScene_ = nextScene_; // move to the next scene
		currentScene_->prevScene_ = toResume; // save the last scene to resume
		currentScene_->load();
		currentScene_->initialize();
		status_ = Status::NONE;
		break;
	}
	case Status::RESTART:
	{
		// simply refresh
		currentScene_->load();
		currentScene_->initialize();
		status_ = Status::NONE;
		break;
	}
	case Status::RESUME:
	{
		Scene* toRelease = currentScene_;
		// Set the current, next, and the last scene to same
		currentScene_ = nextScene_ = currentScene_->prevScene_;
		// initialize the last scene
		delete toRelease;
		status_ = Status::NONE;
		break;
	}
	case Status::RESUME_AND_CHANGE:
	{
		Scene* toRelease = currentScene_;
		// resume to the last scene
		currentScene_ = currentScene_->prevScene_;
		// initialize the last scene
		toRelease->prevScene_ = nullptr;
		// initialize the last scene
		delete toRelease;
		// change the status
		status_ = Status::CHANGE;
		break;
	}
	}

}

void SceneManager::warp_up_scene()
{
	switch (status_) {

		// Pause process
	case Status::PAUSE:

		currentScene_->pause();

		break;

		// The case to quit app
	case Status::QUIT:
		while (currentScene_) {

			Scene* toDelete = currentScene_;
			currentScene_ = currentScene_->prevScene_;
			toDelete->close();
			toDelete->unload();
			toDelete->resume();
			delete toDelete;
			toDelete = nullptr;
		}
		break;

		// The case to resume to last state
	case Status::RESUME:
	case Status::RESUME_AND_CHANGE:
		currentScene_->close();
		currentScene_->unload();
		currentScene_->resume();
		break;

		// The case to restart the current state
		// The case to change to next state
		// The case to resume and change
	case Status::RESTART:
	case Status::CHANGE:
		currentScene_->close();
		currentScene_->unload();
		break;

		// Keep updaring - this should not happen
	case Status::NONE:
	default:
		break;
	}
}

void SceneManager::restart()
{
	if (is_paused())
		jeDebugPrint("Cannot restart on a pause scene\n");

	status_ = Status::RESTART;
}

bool SceneManager::is_paused()
{
	// if the current scene has a scene to resume,
	// then it is pause scene
	return currentScene_->prevScene_ != nullptr;
}

SceneManager::Status SceneManager::get_status(void)
{
	return status_;
}

void SceneManager::set_first_scene(const char* stateName)
{
	auto found = scenes_.find(stateName);
	if (found != scenes_.end())
	{
		nextScene_ = currentScene_ = new Scene(found->first, found->second);
		return;
	}

	jeDebugPrint("[SceneManager::set_first_scene] No such name of scene\n");
}

void SceneManager::set_next_scene(const char* nextState)
{
	// current state is the state
	if (!strcmp(currentScene_->name_.c_str(), nextState))
	{
		jeDebugPrint("[SceneManager::set_next_scene] Cannot set the current scene as the next scene\n");
		return;
	}

	// if current scene is paused from other scene
	if (currentScene_->prevScene_)
	{
		jeDebugPrint("[SceneManager::set_next_scene] Cannot change on a pause scene. Use resume_and_next function\n");
		return;
	}

	auto found = scenes_.find(nextState);
	if (found != scenes_.end())
	{
		nextScene_ = new Scene(found->first, found->second);
		status_ = Status::CHANGE;
		return;
	}
	jeDebugPrint("[SceneManager::set_next_scene] No such name of scene\n");
}

void SceneManager::pause(const char* nextState)
{
	// current state is the state
	if (!strcmp(currentScene_->name_.c_str(), nextState))
	{
		jeDebugPrint("[SceneManager::pause] Cannot set the current scene as the next scene\n");
		return;
	}

	// set the pause state
	auto found = scenes_.find(nextState);
	if (found != scenes_.end())
	{
		nextScene_ = new Scene(found->first, found->second);
		status_ = Status::PAUSE;
		return;
	}
	jeDebugPrint("[SceneManager::pause] No such name of scene\n");
}

void SceneManager::resume()
{
	// Check state to resume
	if (currentScene_->prevScene_)
	{
		status_ = Status::RESUME;
		return;
	}

	jeDebugPrint("[SceneManager::resume] No state to resume\n");
}

void SceneManager::resume_and_next(const char* nextState)
{
	auto found = scenes_.find(nextState);
	if (found != scenes_.end())
	{
		nextScene_ = new Scene(found->first, found->second);
		status_ = Status::RESUME_AND_CHANGE;
	}
}

Scene* SceneManager::get_current_scene(void)
{
	return currentScene_;
}

bool SceneManager::has_scene(const char* stateName)
{
	auto found = scenes_.find(stateName);
	if (found != scenes_.end())
		return true;

	jeDebugPrint("[SceneManager::has_scene] No such name of scene\n");
	return false;
}

void SceneManager::register_scene(const char* dir, const char* sceneName)
{
	scenes_.insert(SceneMap::value_type(sceneName, dir));
}

float SceneManager::get_elapsed_time()
{
	return timer_.get_elapsed_time();
}

unsigned SceneManager::get_frame_per_second()
{
	return frames_;
}

float SceneManager::get_framerate()
{
	return frameTime_;
}

void SceneManager::clear_scenes()
{
	scenes_.clear();
}

jeEnd