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
class Camera;
class GraphicSystem {

	// Prevent to clone this class
	GraphicSystem() = delete;
	~GraphicSystem() = delete;

	jePreventClone(GraphicSystem);

	// todo
	friend class assimpMesh;

	friend class Scene;
	friend class Camera;
	friend class Sprite;
	friend class Model;
	friend class Light;
	friend class Text;
	friend class Emitter;
	friend class DebugRenderer;

	using Renderers = std::vector<Renderer*>;
	using Cameras = std::vector<Camera*>;
	using Lights = std::vector<Light*>;

	struct Grid
	{
		vec3 color = vec3::one;
		bool render = false;
		unsigned size = 10000;
		Renderer::ProjectType prjType = Renderer::ProjectType::PERSPECTIVE;

	private:
		friend class GraphicSystem;
		unsigned texture_ = 0;
	};

public:

	static void set_camera(Camera* camera);
	static Camera* get_camera();

	static Grid grid;
	static vec4 backgroundColor, screenColor;

private:

	static void initialize();
	static void update(float dt);
	static void close();

	static void update_lights(float dt);
	static void render_grid();

	static void add_renderer(Renderer* model);
	static void remove_renderer(Renderer* model);

	static void add_camera(Camera* camera);
	static void remove_camera(Camera* camera);

	static void add_light(Light* light);
	static void remove_light(Light* light);

	static void pause();
	static void resume();

	static Camera* mainCamera_;
	static Renderers renderers_;
	static Cameras cameras_;
	static Lights lights_;

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