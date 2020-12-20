#include <scene_manager.hpp>
#include <debug_tools.hpp>

jeBegin

template<typename SceneType>
inline void SceneManager::register_scene(const char* dir)
{
	const char* typeName = typeid(SceneType).name();
	scenes_.insert(SceneMap::value_type(typeName, dir));
}

template <typename SceneType>
void SceneManager::set_next_scene()
{
	auto found = scenes_.find(typeid(SceneType).name());
	DEBUG_ASSERT(found != scenes_.end(), "No such name of scene!\n");

	// current state is the state
	if (!strcmp(currentScene_->name_.c_str(), found->first))
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

	nextScene_ = new SceneType(found->first, found->second);
	status_ = Status::CHANGE;
}

template <typename SceneType>
void SceneManager::resume_and_next()
{
	auto found = scenes_.find(typeid(SceneType).name());
	DEBUG_ASSERT(found != scenes_.end(), "No such name of scene!\n");
	nextScene_ = new SceneType(found->first, found->second);
	status_ = Status::RESUME_AND_CHANGE;
}

template <typename SceneType>
void SceneManager::set_first_scene()
{
	auto found = scenes_.find(typeid(SceneType).name());
	DEBUG_ASSERT(found != scenes_.end(), "No such name of scene!\n");
	nextScene_ = currentScene_ = new SceneType(found->first, found->second);
}

template <typename SceneType>
void SceneManager::pause()
{
	auto found = scenes_.find(typeid(SceneType).name());
	DEBUG_ASSERT(found != scenes_.end(), "No such name of scene!\n");
	// current state is the state
	if (!strcmp(currentScene_->name_.c_str(), found->first))
	{
		jeDebugPrint("Cannot set the current scene as the next scene\n");
		return;
	}

	// set the pause state
	nextScene_ = new SceneType(found->first, found->second);
	status_ = Status::PAUSE;
}

template <typename SceneType>
bool SceneManager::has_scene()
{
	for (auto scene : scenes_) {

		// If found the one,
		if (!strcmp(typeid(SceneType).name(), scene->first))
			return true;
	}

	jeDebugPrint("No such name of scene\n");
	return false;
}

jeEnd