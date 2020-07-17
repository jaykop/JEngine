/******************************************************************************/
/*!
\file   gl_manager.hpp
\author Jeong Juyong
\par    email: jaykop.jy\@gmail.com
\date   2019/06/15(yy/mm/dd)

\description
Contains the definition of GLManager class
*/
/******************************************************************************/
#pragma once

#include <glew.h>
#define GLEW_STATIC
#include <macros.hpp>
#include <vector>

union SDL_Event;
struct SDL_Window;

jeBegin

class Shader;
class GLManager
{
	// Prevent to clone this class
	GLManager() = delete;
	~GLManager() = delete;

	jePreventClone(GLManager)

	friend class Renderer;
	friend class Sprite;
	friend class Text;
	friend class Emitter;
	friend class Model;
	friend class Light;
	friend class DebugRenderer;
	friend class Application;
	friend class SceneManager;
	friend class GraphicSystem;

	using Shaders = std::vector<Shader*>;

	enum class Target { SCREEN, TEXT, END };
	enum Pipeline { DEBUG, NORMAL, TEXT, PARTICLE, LIGHT, SCREEN, /*GRID,*/ END };

public:

	static float get_width();
	static float get_height();

private:

	static void initialize(float w, float h);
	static void update(SDL_Window* window, const SDL_Event& event);
	static void close();

	static void initialize_shaders();
	static void initialize_buffers();
	static void initialize_framebuffer(); 
	static void delete_buffers();

	static float width_, height_;
	static GLint buffers_, samples_, attributes_;
	static unsigned quadVao_, quadVbo_, quadEbo_,
		drVao_, drVbo_,
		gridVao_, gridVbo_,
		quadIndicesSize_;
	static Shaders shader_;
};

jeEnd