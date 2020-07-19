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
#include <gl_manager.hpp>
#include <input_handler.hpp>
#include <timer.hpp>
#include <debug_tools.hpp>

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
Scenes SceneManager::scenes_;
std::string	SceneManager::firstScene_;
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

		InputHandler::lock_triggered_keys();
	}

	switch (status_) {

		// Pause process
	case Status::PAUSE:
		/*SYSTEM::Pause();*/
		break;

		// The case to quit app
	case Status::QUIT:
		while (currentScene_) {
			Scene* last = currentScene_->prevScene_;
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

void SceneManager::change_scene()
{
	// If the status has changed
	if (status_ == Status::CHANGE
		|| status_ == Status::PAUSE
		|| status_ == Status::RESTART) {

		// save the scene to retusm
		if (status_ == Status::PAUSE) {

			Scene* toResume = currentScene_;
			currentScene_ = nextScene_; // move to the next scene
			currentScene_->prevScene_ = toResume; // save the last scene to resume
		}

		// Just change current state
		else if (status_ == Status::CHANGE)
			currentScene_ = nextScene_;

		// Refresh the scene
		// (Simply restart the current scene)
		currentScene_->load();
		currentScene_->initialize();
	}

	// Resume the scene
	else if (status_ == Status::RESUME) {

		Scene* toRelease = currentScene_;

		// Set the current, next, and the last scene to same
		currentScene_ = nextScene_ = currentScene_->prevScene_;

		// initialize the last scene
		toRelease->prevScene_ = nullptr;
	}

	// Resume and change
	else if (status_ == Status::RESUME_AND_CHANGE) {

		Scene* toRelease = currentScene_;

		// resume to the last scene
		currentScene_ = currentScene_->prevScene_;

		// initialize the last scene
		toRelease->prevScene_ = nullptr;

		// change the status
		status_ = Status::CHANGE;
	}

	// if the current and the next scene are the same,
	// update the status to normal
	if (currentScene_ == nextScene_)
		status_ = Status::NONE;
}

void SceneManager::push_scene(const char* path, const char* stateName)
{
	for (auto it = scenes_.begin(); it != scenes_.end(); ++it)
		DEBUG_ASSERT(strcmp((*it)->name_, stateName), "Trying to add an identical scene!");

	// make a new scene as intended
	Scene* newScene = new Scene(stateName, path);

	// push to the vector
	scenes_.emplace_back(newScene);
}

void SceneManager::push_scene(Scene* scene)
{
	for (auto it = scenes_.begin(); it != scenes_.end(); ++it)
		// If there is already same one, 
		DEBUG_ASSERT(strcmp((*it)->name_, scene->name_), "Trying to add an identical scene!");

	// push to the vector
	scenes_.emplace_back(scene);
}

void SceneManager::pop_scene(const char* stateName)
{
	// find the scene to delete
	for (auto it = scenes_.begin(); it != scenes_.end(); ++it) {

		// get the same scene with the name given 
		if (!strcmp((*it)->name_, stateName)) {
			delete (*it);		// return the memory
			scenes_.erase(it);	// remove from the vector
			break;
		}
	}
}

void SceneManager::set_first_scene(const char* stateName)
{
	if (firstScene_.empty())
	{
		firstScene_.assign(stateName);

		// find the correct scene from the vector
		for (auto it = scenes_.begin(); it != scenes_.end(); ++it)
		{
			if (!strcmp((*it)->name_, firstScene_.c_str()))
			{
				nextScene_ = currentScene_ = (*it);
				break;
			}
		}
	}

	else
		jeDebugPrint("The frist scene has been set already!\n");

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

void SceneManager::set_next_scene(const char* nextState)
{
	// current state is the state
	if (!strcmp(currentScene_->name_, nextState))
	{
		jeDebugPrint("Cannot set the current scene as the next scene\n");
		return;
	}

	// if current scene is paused from other scene
	if (currentScene_->prevScene_)
	{
		jeDebugPrint("Cannot change on a pause scene. Use resume_and_next function\n");
		return;
	}

	if (has_scene(nextState)) {
		nextScene_ = get_scene(nextState);
		status_ = Status::CHANGE;
	}
}

void SceneManager::pause(const char* nextState)
{
	// current state is the state
	if (!strcmp(currentScene_->name_, nextState))
	{
		jeDebugPrint("Cannot set the current scene as the next scene\n");
		return;
	}

	// set the pause state
	if (has_scene(nextState)) {
		nextScene_ = get_scene(nextState);
		status_ = Status::PAUSE;
	}
}

void SceneManager::resume()
{
	// Check state to resume
	if (currentScene_->prevScene_ == nullptr)
	{
		jeDebugPrint("No state to resume\n");
		return;
	}

	status_ = Status::RESUME;
}

void SceneManager::resume_and_next(const char* nextState)
{
	if (has_scene(nextState)) {
		status_ = Status::RESUME_AND_CHANGE;
		nextScene_ = get_scene(nextState);
	}
}

Scene* SceneManager::get_current_scene(void)
{
	return currentScene_;
}

Scene* SceneManager::get_scene(const char* stateName)
{
	// find the scene
	for (auto it : scenes_)
		if (!strcmp(stateName, it->name_))
			return it;

	// If there is no,
	jeDebugPrint("No such name of scene\n");
	return nullptr;
}

bool SceneManager::has_scene(const char* stateName)
{
	for (auto scene : scenes_) {

		// If found the one,
		if (!strcmp(stateName, scene->name_))
			return true;
	}

	jeDebugPrint("No such name of scene\n");
	return false;
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
	// Remove all states from the vector
	for (auto& it : scenes_)
		delete it;

	scenes_.clear();
}

jeEnd