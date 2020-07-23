#include "level5.h"
#include "scene_controller.h"
#include "camera_controller.h"
#include "pch.h"
#include "model.hpp"
#include "mesh.hpp"
#include "assimpModel.hpp"

jeBegin

void Level5::initialize()
{
	init_basic();
	// init_models();

	// base init
	Scene::initialize();
}

void Level5::update(float dt)
{
	// base update
	Scene::update(dt);
}

void Level5::close()
{
	// base close
	Scene::close();
}

void Level5::init_basic()
{
	// add camera
	Object* camera = ObjectManager::create_object("main_camera");
	camera->add_component<Camera>();
	camera->add_component<CameraController>();
	camera->get_component<Camera>()->position = vec3(0.f, 0.f, 10.f);
	register_object(camera);

	GraphicSystem::set_camera(camera->get_component<Camera>());

	// scene controller
	Object* controller = ObjectManager::create_object("scene_controller");
	controller->add_component<SceneController>();
	register_object(controller);
}

void Level5::init_lights()
{

}

jeEnd
