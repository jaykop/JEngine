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
#include <SDL_events.h>
#include <gl_manager.hpp>
#include <debug_tools.hpp>
#include <shader.hpp>
#include <vertex.hpp>

#include <application.hpp>

jeBegin

const std::vector<float> quadVertices =
{
	-.5f, .5f, 0.0f , 0.f, 0.f, 1.f,  0.0f, 0.0f ,
	-.5f, -.5f, 0.0f , 0.f, 0.f, 1.f,  0.0f, 1.0f,
	.5f, -.5f, 0.0f , 0.f, 0.f, 1.f,  1.0f, 1.0f,
	.5f,  .5f, 0.0f , 0.f, 0.f, 1.f,  1.0f, 0.0f
};

const std::vector<unsigned> quadIndices = { 2, 0, 1, 2, 3, 0 };
const vec3 stdResolution(1.f / 800.f, 1.f / 600.f, 1.f);

vec3 GLManager::resScaler_;
GLManager::Shaders GLManager::shader_;
int GLManager::widthStart_ = 0, GLManager::heightStart_ = 0;
float GLManager::width_ = 0.f, GLManager::height_ = 0.f;
GLint GLManager::buffers_, GLManager::samples_, GLManager::attributes_;
unsigned GLManager::quadVao_ = 0, GLManager::quadVbo_ = 0, GLManager::quadEbo_ = 0,
GLManager::drVao_ = 0, GLManager::drVbo_ = 0,
GLManager::quadIndicesSize_ = 6, GLManager::gridVerticeSize_ = 48;

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
	initialize_buffers();

	vec3 windowSize(width_, height_, 1.f);
	resScaler_ = windowSize * stdResolution;
	resScaler_.set(0.5f / resScaler_.x, 0.5f / resScaler_.y, 1.f);
}

void GLManager::update(SDL_Window* window, const SDL_Event& event)
{
	switch (event.type)
	{
	case SDL_WINDOWEVENT_RESIZED:
	{
		//int w, h;

		//SDL_GetWindowSize(window, &w, &h);
		//auto& appData = Application::get_appdata();

		//appData.width = w;
		//appData.width = h;

		//if (appData.isFullscreen)
		//{
		//	GLManager::widthStart_ = appData.displayWidth / 2 - appData.width / 2;
		//	GLManager::heightStart_ = appData.displayHeight / 2 - appData.height / 2;
		//}

		//else
		//{
		//	GLManager::widthStart_ = GLManager::heightStart_ = 0;
		//}

		//// Update the projection size by window screen size
		//width_ = float(w), height_ = float(h);
		//vec3 windowSize(width_, height_, 1.f);
		//resScaler_ = windowSize * stdResolution;
		//resScaler_.set(0.5f / resScaler_.x, 0.5f / resScaler_.y, 1.f);

		break;
	}
	case SDL_WINDOWEVENT_CLOSE:
	{
		// TODO
		break;
	}
	case SDL_WINDOWEVENT_MOVED:
	{
		SDL_GetWindowPosition(window, &widthStart_, &heightStart_);

		break;
	}
	}	

	vec3 windowSize(width_, height_, 1.f);
	resScaler_ = windowSize * stdResolution;
	resScaler_.set(0.5f / resScaler_.x, 0.5f / resScaler_.y, 1.f);
}

void GLManager::close()
{
	delete_buffers();

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
		newShader->combine_shaders(i);

		shader_.push_back(newShader);
	}

	// remove shader directories
	Shader::vsDirectory_.clear();
	Shader::fsDirectory_.clear();

	jeDebugPrint("*GLManager - Compiled and linked shaders.\n");
}

void GLManager::initialize_buffers()
{
	/**************************** QUAD BUFFER ******************************/
	// generate vertex array
	glGenVertexArrays(1, &quadVao_);
	glBindVertexArray(quadVao_);

	// generate vertex buffer
	glGenBuffers(1, &quadVbo_);
	glBindBuffer(GL_ARRAY_BUFFER, quadVbo_);

	// set vertex data
	glBufferData(GL_ARRAY_BUFFER, quadVertices.size() * sizeof(float), &quadVertices[0], GL_STATIC_DRAW);

	// vertex position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// normals of vertices
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// texture coordinate position
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// generate index buffer
	glGenBuffers(1, &quadEbo_);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadEbo_);

	// set index data
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, quadIndices.size() * sizeof(unsigned), &quadIndices[0], GL_STATIC_DRAW);

	// unbind buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	/**************************** DEBUG RENDERER BUFFER ******************************/

	glGenVertexArrays(1, &drVao_);
	glBindVertexArray(drVao_);

	glGenBuffers(1, &drVbo_);
	glBindBuffer(GL_ARRAY_BUFFER, drVbo_);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		reinterpret_cast<void*>(offsetof(Vertex, Vertex::position)));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		reinterpret_cast<void*>(offsetof(Vertex, Vertex::normal)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		reinterpret_cast<void*>(offsetof(Vertex, Vertex::texCoords)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		reinterpret_cast<void*>(offsetof(Vertex, Vertex::color)));
	glEnableVertexAttribArray(3);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void GLManager::delete_buffers()
{
	glDeleteVertexArrays(1, &quadVao_);
	glDeleteBuffers(1, &quadVbo_);
	glDeleteBuffers(1, &quadEbo_);

	glDeleteVertexArrays(1, &drVao_);
	glDeleteBuffers(1, &drVbo_);
}

float GLManager::get_width() { return width_; }

float GLManager::get_height() { return height_; }

jeEnd