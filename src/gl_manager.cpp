/******************************************************************************/
/*!
\file   gl_manager.cpp
\author Jeong Juyong
\par    email: jaykop.jy\@gmail.com
\date   2019/06/15(yy/mm/dd)

\description
Contains the methods of GLManager class
*/
/******************************************************************************/

#include <SDL_Video.h>
#include <gl_manager.hpp>
#include <debug_tools.hpp>
#include <shader.hpp>
#include <SDL_events.h>

jeBegin

GLManager::Shaders GLManager::shader_;
float GLManager::width_ = 0.f, GLManager::height_ = 0.f;
GLint GLManager::buffers_, GLManager::samples_, GLManager::attributes_;

void GLManager::initialize(float w, float h)
{
	width_ = w, height_ = h;

	// force GLEW to use a modern OpenGL method
	glewExperimental = GL_TRUE;

	// before using shader, initialize glew.
#if defined(_DEBUG)
	DEBUG_ASSERT(glewInit() == GLEW_OK, "Failed to initialize GLEW");
#else
	glewInit();
#endif

	GLuint vertexBuffer;
	glGenBuffers(1, &vertexBuffer);
	// printf("%u\n", vertexBuffer);

	// show GL version info
	const GLubyte* pRenderer_ = glGetString(GL_RENDERER);
	const GLubyte* pVendor_ = glGetString(GL_VENDOR);
	const GLubyte* pVersion_ = glGetString(GL_VERSION);
	const GLubyte* pGlslVersion_ = glGetString(GL_SHADING_LANGUAGE_VERSION);

	glGetIntegerv(GL_SAMPLE_BUFFERS, &buffers_);
	glGetIntegerv(GL_SAMPLES, &samples_);

	jeDebugPrint("*GLManager - GL Vendor: %s / GL Renderer: %s\n", pVendor_, pRenderer_);
	jeDebugPrint("*GLManager - GL Version: %s\n", pVersion_);
	jeDebugPrint("*GLManager - GLSL Version: %s\n", pGlslVersion_);
	jeDebugPrint("*GLManager - GL Samples: %d / GL Sample Buffers: %d\n", samples_, buffers_);

	// show how many attributes are available
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &attributes_);
	jeDebugPrint("*GLManager - Maximum number of vertex attributes supported: %d\n", attributes_);

	initialize_shaders();
}

void GLManager::update(SDL_Window* window, const SDL_Event& event)
{
	if (event.type == SDL_WINDOWEVENT_RESIZED) {
		int w, h;
		SDL_GetWindowSize(window, &w, &h);
		width_ = float(w), height_ = float(h);
	}
	else if (event.type == SDL_WINDOWEVENT_CLOSE) {
		// TODO
	}

	glViewport(0, 0, (GLsizei)width_, (GLsizei)height_);
}

void GLManager::close()
{
	for (unsigned i = 0; i < Pipeline::END; ++i) 
		delete shader_[i];

	shader_.clear();
}

void GLManager::initialize_shaders()
{
	// do shader stuff
	for (unsigned int i = 0; i < Pipeline::END; ++i) {

		Shader* newShader = new Shader;
		newShader->create_shader(Shader::vsDirectory_[i], Shader::Type::VERTEX);

		// TODO
		// Work on geometry shader
		//shader_[i]->CreateShader(Shader::m_geometryShader[i], Shader::JE_GEOMETRY);

		newShader->create_shader(Shader::fsDirectory_[i], Shader::Type::PIXEL);
		newShader->combine_shaders();

		shader_.push_back(newShader);
	}

	// remove shader directories
	Shader::vsDirectory_.clear();
	Shader::fsDirectory_.clear();

	jeDebugPrint("*GLManager - Compiled and linked shaders.\n");
}

float GLManager::get_width() { return width_; }

float GLManager::get_height() { return height_; }

//sf::ContextSettings GLManager::get_context_settings()
//{
//
//
//	return settings;
//}

jeEnd