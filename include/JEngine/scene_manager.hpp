/******************************************************************************/
/*!
\file   application.cpp
\author Jeong Juyong
\par    email: jaykop.jy\@gmail.com
\date   2019/06/02(yy/mm/dd)

\description
Contains the definition of SceneManager class
*/
/******************************************************************************/

#pragma once

// Main scene manager class
#include <vector>
#include <timer.hpp>
#include <unordered_map>

union SDL_Event;
struct SDL_Window;

jeBegin

class Scene;
using SceneMap = std::unordered_map<const char*, const char*>;

class SceneManager {

	// Prevent to clone this class
	SceneManager() = delete;
	~SceneManager() = delete;

	jePreventClone(SceneManager)

	friend class Application;
	friend class AssetManager;
	
	// enum indicates the status of the current scene
	enum class Status {NONE, RESTART, PAUSE, RESUME, CHANGE, RESUME_AND_CHANGE, QUIT};

public:

	template <typename SceneType>
	static void register_scene(const char* dir);
	template <typename SceneType>
	static void set_next_scene();
	template <typename SceneType>
	static void resume_and_next();
	template <typename SceneType>
	static void set_first_scene();
	template <typename SceneType>
	static bool has_scene();
	template <typename SceneType>
	static void pause();

	// methods to control the scenes
	static void restart();
	static void resume();
	static bool is_paused();
	static void pause(const char* nextState);
	static void register_scene(const char* sceneName, const char* dir);
	static void set_next_scene(const char* nextState);
	static void resume_and_next(const char* nextState);
	static void set_first_scene(const char* stateName);

	static Status get_status(void);
	static Scene* get_current_scene(void);
	static bool	has_scene(const char* stateName);

	// methods to get frame and time info
	static float get_elapsed_time();
	static float get_framerate();
	static unsigned	get_frame_per_second();

private:

	static bool initialize(SDL_Window* window);
	static void update(SDL_Event* event);
	static void close();

	static void change_scene();
	static void warp_up_scene();
	static void clear_scenes();

	// frame and timer
	static int frames_;
	static Timer timer_;
	static float frameTime_;

	// scene info
	static SceneMap scenes_;
	static Status status_;
	static Scene *currentScene_, *nextScene_;
	static SDL_Window *window_;

};

jeEnd

#include <scene_manager.inl>