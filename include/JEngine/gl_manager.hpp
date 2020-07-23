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
#include <vec3.hpp>

union SDL_Event;
struct SDL_Window;

jeBegin

class Shader;
class GLManager
{
	// Prevent to clone this class
	GLManager() = delete;
	~GLManager() = delete;

	jePreventClone(GLManager);

	// todo:
	friend class assimpMesh;

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



public:

	static float get_width();
	static float get_height();

private:

	static void initialize();
	static void close();

	static void initialize_shaders();
	static void initialize_buffers();
	static void initialize_framebuffer(); 
	static void delete_buffers();

};

jeEnd