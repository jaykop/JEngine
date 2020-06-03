#include <gl_manager.hpp>
#include <application.hpp>
#include <random.hpp>
#include <json_parser.hpp>
#include <debug_tools.hpp>
#include <scene_manager.hpp>
#include <asset_manager.hpp>
#include <input_handler.hpp>
#include <SDL_image.h>

jeBegin

//////////////////////////////////////////////////////////////////////////
// static variables
//////////////////////////////////////////////////////////////////////////
bool Application::run_ = true;
SDL_Event		Application::event_;
SDL_Window*		Application::window_ = nullptr;
SDL_Surface		*Application::surface_ = nullptr, *Application::icon_= nullptr;
SDL_GLContext	Application::context_ = nullptr;
Application::AppData	Application::data_ = { "demo", "../resource/ico/main.ico", false, 800, 600 };

void Application::run()
{
	// pop a console window
	DEBUG_LEAK_CHECKS(-1);
	DEBUG_CREATE_CONSOLE();

	// if initialization succeeded, run the app
	if (initialize()) {

		update();
		close();
	}

	// destroy the console window
	DEBUG_DESTROY_CONSOLE();
	
}

void Application::activate_vsync(bool on)
{
	SDL_GL_SetSwapInterval(on);
}

Application::AppData Application::get_appdata()
{
	return data_;
}

bool Application::initialize()
{
	// load app init data
	JsonParser::read_file(AssetManager::initDirectory_.c_str());

	const rapidjson::Value& title = JsonParser::get_document()["Title"];
	const rapidjson::Value& fullscreen = JsonParser::get_document()["Fullscreen"];
	const rapidjson::Value& width = JsonParser::get_document()["Width"];
	const rapidjson::Value& height = JsonParser::get_document()["Height"];
	const rapidjson::Value& icon = JsonParser::get_document()["Icon"];

	if (title.IsString() && fullscreen.IsBool()
		&& width.IsInt() && height.IsInt()
		&& icon.IsString()) {

		data_.title.assign(title.GetString());
		data_.icon.assign(icon.GetString());
		data_.isFullscreen = fullscreen.GetBool();
		data_.width = width.GetInt();
		data_.height = height.GetInt();
	}

	Random::seed();

	// Check right init
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
		// Print error message
		jeDebugPrint("!Application - SDL could not initialize. SDL_Error: %s\n", SDL_GetError());
		return false;
	}


	// Initialize png loading
	if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
	{
		jeDebugPrint("!Application - SDL_image could not initialize. SDL_image Error: %s\n", IMG_GetError());
		return false;
	}

	// Call opengl 4.5
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	//SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	//SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	//SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

	SDL_DisplayMode current;
	SDL_GetCurrentDisplayMode(0, &current);

	//Create window
	window_ = SDL_CreateWindow(data_.title.c_str(), 
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		data_.width, data_.height, SDL_WINDOW_OPENGL);

	if (!window_) {
		jeDebugPrint("!Application - Window could not be created. SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	// Get context
	context_ = SDL_GL_CreateContext(window_);

	// Set window icon
	icon_ = IMG_Load(data_.icon.c_str());
	SDL_SetWindowIcon(window_, icon_);
	SDL_SetWindowFullscreen(window_, data_.isFullscreen);

	// Get window surface
	surface_ = SDL_GetWindowSurface(window_);

	// Fill the surface white
	SDL_FillRect(surface_, nullptr, SDL_MapRGB(surface_->format, 0xFF, 0xFF, 0xFF));

	AssetManager::load_shaders();
	GLManager::initialize(float(data_.width), float(data_.height));

	// initialize components and assets
	AssetManager::set_bulit_in_components();
	AssetManager::load_assets();

	// initialize key map
	InputHandler::initialize();

	return true;
}

// the process handling scene manager is handled here
void Application::update()
{
	// If the intiailization does not have done properly,
	// stop updating the scene manager
	if (!SceneManager::initialize(window_))
		return;

	// update the window 
	while (run_)
		SceneManager::update(&event_); // update the scene

	SceneManager::close(); // close the scene manager
}

void Application::close()
{
	// Delete icon_
	SDL_FreeSurface(icon_);

	// Destroy
	SDL_GL_DeleteContext(context_);

	//Destroy window
	SDL_DestroyWindow(window_);

	//Quit SDL subsystems
	SDL_Quit();

	InputHandler::close();
	AssetManager::unload_assets();
	GLManager::close();
}

void Application::quit()
{
	run_ = false;
	SceneManager::status_ = SceneManager::Status::QUIT;
}

jeEnd
