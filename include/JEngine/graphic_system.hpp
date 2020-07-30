/******************************************************************************/
/*!
\file   graphic_system.hpp
\author Jeong Juyong
\par    email: jaykop.jy\@gmail.com
\date   2019/06/15(yy/mm/dd)

\description
Contains the definition of GraphicSystem class
*/
/******************************************************************************/

#pragma once
#include <stack>
#include <macros.hpp>
#include <vector>
#include <vec4.hpp>
#include <renderer.hpp>

jeBegin

class Renderer;
class GraphicSystem {

	// Prevent to clone this class
	GraphicSystem() = delete;
	~GraphicSystem() = delete;

	jePreventClone(GraphicSystem);

	friend class Scene;
	friend class Camera;
	friend class Mesh;
	friend class Sprite;
	friend class Model;
	friend class Light;
	friend class Text;
	friend class Emitter;
	friend class DebugRenderer;
	friend class Application;

	using Renderers = std::vector<Renderer*>;
	using Cameras = std::vector<Camera*>;
	using Lights = std::vector<Light*>;
	using Shaders = std::vector<Shader*>;

	// enum class Target { SCREEN, TEXT, END };
	enum Pipeline { SPRITE, TEXT, PARTICLE, MODEL, LIGHT, DEBUG, GRID, SKYBOX, ENVIRONMENT, SCREEN, END };

	struct Grid
	{
		vec3 color = vec3::one;
		bool render = false;
		float scale = 10.f, thickness = 0.1f;
		int divisions = 10;
		Renderer::ProjectType prjType = Renderer::ProjectType::PERSPECTIVE;
	};

	struct Skybox
	{
		unsigned texture = 0;
		unsigned textures[6] = { 0 };
		float scale = 10.f;
		vec3 color = vec3::one;
	};

public:

	static void recompile_shaders();

	static float get_width();
	static float get_height();
	static unsigned get_num_of_lights();
	static void set_camera(Camera* camera);
	static Camera* get_camera();

	static Grid grid;
	static Skybox skybox;
	static vec4 backgroundColor, screenColor;

private:

	static void initialize_fbo();
	static void close_fbo();

	static void initialize_shaders();
	static void close_shaders();

	static void initialize_graphics();
	static void close_graphics();

	static void initialize();
	static void update(float dt);
	static void close();

	static void update_lights(float dt);
	static void render_grid();
	static void render_skybox();
	static void render_copy(float dt);

	static void add_renderer(Renderer* model);
	static void remove_renderer(Renderer* model);

	static void add_camera(Camera* camera);
	static void remove_camera(Camera* camera);

	static void add_light(Light* light);
	static void remove_light(Light* light);

	static void pause();
	static void resume();

	static vec3 resScaler_;
	static Camera* mainCamera_;

	static float width_, height_;
	static int widthStart_, heightStart_;
	static unsigned quadVao_, quadVbo_, quadEbo_,
		drVao_, drVbo_,	quadIndicesSize_,
		skyboxVao_, skyboxVbo_, 
		fbo_, environmentTextures_[6];

	static Lights lights_;
	static Shaders shader_;
	static Cameras cameras_;
	static Renderers renderers_;

	struct Graphic
	{
		Grid grid;
		vec4 backgroundColor, screenColor;
		Camera* mainCamera;
		Renderers renderers;
		Cameras cameras;
		Lights lights;
	};

	static std::stack<Graphic> graphicStack_;
};

jeEnd