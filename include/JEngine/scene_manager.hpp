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
#include <string>
#include <timer.hpp>

union SDL_Event;
struct SDL_Window;

jeBegin

class Scene;
class ObjectContainer;
using Scenes = std::vector<Scene*>;

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

	static void add_scene(Scene* scene);

	// methods to control the scenes
	static void restart();
	static void resume();
	static void pause(const char* nextState);
	static bool is_paused();
	static void set_next_scene(const char* nextState);
	static void resume_and_next(const char* nextState);
	static void set_first_scene(const char* stateName);

	static Status get_status(void);
	static Scene* get_current_scene(void);
	static Scene* get_scene(const char* stateName);
	static bool	has_scene(const char* stateName);

	// methods to get frame and time info
	static float get_elapsed_time();
	static float get_framerate();
	static unsigned	get_frame_per_second();

private:

	static bool initialize(SDL_Window* window);
	static void update(SDL_Event* event);
	static void close();

	// methods to control scenes
	static void push_scene(const char* path, const char* stateName);
	static void pop_scene(const char* stateName);

	static void change_scene();
	static void clear_scenes();

	// frame and timer
	static int frames_;
	static Timer timer_;
	static float frameTime_;

	// scene info
	static Scenes scenes_;
	static Status status_;
	static Scene *currentScene_, *nextScene_;
	static SDL_Window *window_;
	static std::string	firstScene_;

};

jeEnd