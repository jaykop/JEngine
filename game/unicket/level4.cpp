#include "level4.h"
#include "scene_controller.h"
#include "camera_controller.h"
#include "pch.h"
#include "model.hpp"
#include "mesh.hpp"
#include "model.hpp"

jeBegin

void Level4::initialize()
{
	init_basic();
	// init_models();

	// base init
	Scene::initialize();
}

void Level4::update(float dt)
{
	// base update
	Scene::update(dt);
}

void Level4::close()
{
	// base close
	Scene::close();
}

void Level4::init_basic()
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

void Level4::init_models()
{
	model1 = ObjectManager::create_object("cube");
	model1->add_component<Model>();
	auto* md = model1->get_component<Model>();
	md->set_meshes(AssetManager::get_meshes("cube"));
	// md->set_draw_mode(GL_TRIANGLES);
	register_object(model1);
	// model2 = ObjectManager::create_object("sphere");
}

jeEnd
