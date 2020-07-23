#include <glew.h>
#define GLEW_STATIC
#include <application.hpp>
#include <random.hpp>
#include <json_parser.hpp>
#include <debug_tools.hpp>
#include <scene_manager.hpp>
#include <asset_manager.hpp>
#include <input_handler.hpp>
#include <SDL_image.h>
#include <graphic_system.hpp>

jeBegin

//////////////////////////////////////////////////////////////////////////
// static variables
//////////////////////////////////////////////////////////////////////////
bool Application::run_ = true;
SDL_Event Application::event_;
SDL_Window* Application::window_ = nullptr;
SDL_Surface	*Application::surface_ = nullptr, *Application::icon_= nullptr;
SDL_GLContext Application::context_ = nullptr;
Application::AppData Application::data_ = { "demo", "resource/ico/main.ico", false, false, 800, 600, 0, 0 };

const vec3 stdResolution(1.f / 800.f, 1.f / 600.f, 1.f);

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

bool Application::cursor_hidden()
{
	return data_.cursorHidden;
}

SDL_Window* Application::get_window()
{
	return window_;
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
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	//SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	//SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	//SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

	SDL_DisplayMode current;
	SDL_GetCurrentDisplayMode(0, &current);

	SDL_DisplayMode display;
	SDL_GetCurrentDisplayMode(0, &display);
	data_.displayWidth = display.w;
	data_.displayHeight = display.h;

	//Create window
	window_ = SDL_CreateWindow(data_.title.c_str(), 
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		data_.displayWidth, data_.displayHeight, SDL_WINDOW_OPENGL);

	if (!window_) {
		jeDebugPrint("!Application - Window could not be created. SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	// Get context
	context_ = SDL_GL_CreateContext(window_);

	// Set window icon
	icon_ = IMG_Load(data_.icon.c_str());
	SDL_SetWindowIcon(window_, icon_);

	//TODO: SDL FULLSCREEN ISSUE
	//SDL_SetWindowFullscreen(window_, data_.isFullscreen);
	set_fullscreen(data_.isFullscreen);

	// Get window surface								
	surface_ = SDL_GetWindowSurface(window_);

	// Fill the surface white
	SDL_FillRect(surface_, nullptr, SDL_MapRGB(surface_->format, 0xFF, 0xFF, 0xFF));

	gl_initialize();

	// initialize components and assets
	AssetManager::load_shaders();
	AssetManager::set_bulit_in_components();
	AssetManager::load_assets();

	// initialize key map
	InputHandler::initialize();
	GraphicSystem::initialize_graphics();

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
	GraphicSystem::close_graphics();
	JsonParser::close();
}

void Application::quit()
{
	run_ = false;
	SceneManager::status_ = SceneManager::Status::QUIT;
}

//TODO: SDL FULLSCREEN ISSUE
void Application::set_fullscreen(bool fullscreen)
{
	if (fullscreen)
	{
		SDL_SetWindowPosition(window_, 0, 0);
		SDL_SetWindowSize(window_, data_.displayWidth, data_.displayHeight);

		float offset = static_cast<float>(data_.displayHeight) / data_.height;
		int iWidth = static_cast<int>(offset * static_cast<float>(data_.width));

		GraphicSystem::widthStart_ = data_.displayWidth / 2 - iWidth / 2;
		GraphicSystem::heightStart_ = 0;
		GraphicSystem::width_ = static_cast<float>(iWidth);
		GraphicSystem::height_ = static_cast<float>(data_.displayHeight);

	}

	else
	{
		SDL_SetWindowPosition(window_, 
			data_.displayWidth /2 - data_.width / 2,
			data_.displayHeight / 2 - data_.height / 2);
		SDL_SetWindowSize(window_, data_.width, data_.height);

		GraphicSystem::width_ = static_cast<float>(data_.width);
		GraphicSystem::height_ = static_cast<float>(data_.height);

		GraphicSystem::widthStart_ = GraphicSystem::heightStart_ = 0;
	}

	data_.isFullscreen = fullscreen;
}

void Application::hide_cursor(bool hide)
{
	data_.cursorHidden = hide;
	SDL_bool enable = hide ? SDL_FALSE : SDL_TRUE;
	// SDL_SetRelativeMouseMode(enable);
	SDL_ShowCursor(enable);
}

void Application::event_update()
{
	switch (event_.type)
	{
	case SDL_WINDOWEVENT_RESIZED:
	{
		break;
	}
	case SDL_WINDOWEVENT_CLOSE:
	{
		// TODO
		break;
	}
	case SDL_WINDOWEVENT_MOVED:
	{
		break;
	}
	}

	vec3 windowSize(static_cast<float>(Application::get_appdata().width),
		static_cast<float>(Application::get_appdata().height), 1.f);
	GraphicSystem::resScaler_ = windowSize * stdResolution;
	GraphicSystem::resScaler_.set(0.5f / GraphicSystem::resScaler_.x, 
		0.5f / GraphicSystem::resScaler_.y, 1.f);
}

void Application::gl_initialize()
{
	// force GLEW to use a modern OpenGL method
	glewExperimental = GL_TRUE;

	// before using shader, initialize glew.
#if defined(_DEBUG)
	DEBUG_ASSERT(glewInit() == GLEW_OK, "Failed to initialize GLEW");
#else
	glewInit();
#endif

	GLuint buffer; 
	glGenBuffers(1, &buffer);

	// todo:
	glDeleteBuffers(1, &buffer);

	// show GL version info
	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* vendor = glGetString(GL_VENDOR);
	const GLubyte* version = glGetString(GL_VERSION);
	const GLubyte* glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);

	GLint buffers, samples, attributes;

	glGetIntegerv(GL_SAMPLE_BUFFERS, &buffers);
	glGetIntegerv(GL_SAMPLES, &samples);

	jeDebugPrint("*GL Vendor: %s / GL Renderer: %s\n", vendor, renderer);
	jeDebugPrint("*GL Version: %s\n", version);
	jeDebugPrint("*GLSL Version: %s\n", glslVersion);
	jeDebugPrint("*GL Samples: %d / GL Sample Buffers: %d\n", samples, buffers);

	// show how many attributes are available
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &attributes);
	jeDebugPrint("*Maximum number of vertex attributes supported: %d\n", attributes);

}

jeEnd
