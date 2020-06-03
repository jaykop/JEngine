/******************************************************************************/
/*!
\file   scene_controller.cpp
\author Jeong Juyong
\par    email: jaykop.jy\@gmail.com
\date   2019/06/02(yy/mm/dd)

\description
Contains the methods of SceneManager class
*/
/******************************************************************************/

#include <scene.hpp>
#include <scene_manager.hpp>
#include <debug_tools.hpp>

jeBegin

Scenes SceneManager::scenes_;
std::string	SceneManager::firstScene_;
Scene* SceneManager::nextScene_ = nullptr;

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
			currentScene_->lastScene_ = toResume; // save the last scene to resume
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
		currentScene_ = nextScene_ = currentScene_->lastScene_;

		// initialize the last scene
		toRelease->lastScene_ = nullptr;
	}

	// Resume and change
	else if (status_ == Status::RESUME_AND_CHANGE) {
		
		Scene* toRelease = currentScene_;
		
		// resume to the last scene
		currentScene_ = currentScene_->lastScene_;

		// initialize the last scene
		toRelease->lastScene_ = nullptr;

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
	bool sameOne = false;
	for (auto it = scenes_.begin();	it != scenes_.end(); ++it) {

		// If there is already same one, 
		if (!strcmp((*it)->name_, stateName)) {
			jeDoPrint("Trying to add an identical scene!");
			sameOne = true;
			break;
		}
	}

	// prevent to have duplicated states
	if (!sameOne) {

		// make a new scene as intended
		Scene* newScene = new Scene(stateName);
		newScene->directory_ = path;

		// push to the vector
		scenes_.push_back(newScene);

		// if there is only single scene,
		// make it the starting scene
		if (scenes_.size() == 1)
			set_first_scene(stateName);
	}
}

void SceneManager::add_scene(Scene* scene)
{
	bool sameOne = false;
	for (auto it = scenes_.begin(); it != scenes_.end(); ++it) {

		// If there is already same one, 
		if (!strcmp((*it)->name_, scene->name_)) {
			jeDoPrint("Trying to add an identical scene!");
			sameOne = true;
			break;
		}
	}

	// prevent to have duplicated states
	if (!sameOne) {

		// push to the vector
		scenes_.push_back(scene);
		jeDoPrint("Added scene->name_");

		// if there is only single scene,
		// make it the starting scene
		if (scenes_.size() == 1)
			set_first_scene(scene->name_);
	}
}

void SceneManager::pop_scene(const char* stateName)
{
	// find the scene to delete
	for (auto it = scenes_.begin();	it != scenes_.end(); ++it) {

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
	// if the first scene is blank,
	// take this parameter
	if (firstScene_.empty())
		firstScene_.assign(stateName);

	// unless, find the correct scene from the vector
	for (auto it = scenes_.begin(); it != scenes_.end(); ++it) {

		if (!strcmp((*it)->name_, firstScene_.c_str()))
			nextScene_ = currentScene_ = (*it);
	}
}

void SceneManager::restart()
{
	if (is_paused())
		jeDoPrint("Cannot restart on a pause scene");

	status_ = Status::RESTART;
}

bool SceneManager::is_paused()
{
	// if the current scene has a scene to resume,
	// then it is pause scene
	return currentScene_->lastScene_ != nullptr;
}

SceneManager::Status SceneManager::get_status(void)
{
	return status_;
}

void SceneManager::set_next_scene(const char* nextState)
{
	// current state is the state
	if (strcmp(currentScene_->name_, nextState)) 
		jeDoPrint("Cannot set the current scene as the next scene");
	
	// if there is no scene to resume
	if (currentScene_->lastScene_)
		jeDoPrint("Cannot change on a pause scene.\nUse resume_and_next function");

	if (has_scene(nextState)) {
		nextScene_ = get_scene(nextState);
		status_ = Status::CHANGE;
	}
		
}

void SceneManager::pause(const char* nextState)
{
	// current state is the state
	if (strcmp(currentScene_->name_, nextState))
		jeDoPrint("Cannot set the current scene as the next scene");

	// set the pause state
	if (has_scene(nextState)) {
		nextScene_ = get_scene(nextState);
		status_ = Status::PAUSE;
	}
}

void SceneManager::resume()
{
	// Check state to resume
	if (currentScene_->lastScene_ == nullptr)
		jeDoPrint("No state to resume");

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
	jeDoPrint("No such name of scene");
	return nullptr;
}

bool SceneManager::has_scene(const char* stateName)
{
	bool found = false;
	for (auto scene : scenes_) {

		// If found the one,
		if (!strcmp(stateName, scene->name_)) {
			found = true;
			return found;
		}
	}

	jeDoPrint("No such name of scene");
	return found;
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
	for (auto it = scenes_.begin(); it != scenes_.end(); )
		delete (*it++);

	scenes_.clear();
}

jeEnd