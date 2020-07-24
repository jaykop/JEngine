#include "level5.h"
#include "scene_controller.h"
#include "camera_controller.h"
#include "pch.h"
#include "light.hpp"
#include "model.hpp"

jeBegin

void Level5::initialize()
{
	init_basic();
	init_lights();
	init_models();

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
	Object* light1 = ObjectManager::create_object("light1");
	light1->add_component<Light>();
	auto* trans = light1->get_component<Transform>();
	trans->scale.set(1.f, 1.f, 1.f);
	trans->position.set(0.f, 0.f, 0.f);
	register_object(light1);
}


void Level5::init_models()
{
	auto* cube = ObjectManager::create_object("cube");
	cube->add_component<Model>();
	auto* md3 = cube->get_component<Model>();
	auto* trans3 = cube->get_component<Transform>();
	md3->set_meshes(AssetManager::get_meshes("cube"));
	md3->color.set(1, 0, 0, 1);
	trans3->position.set(-3.f, 0.f, 0.f);
	trans3->scale.set(1.f, 1.f, 1.f);
	register_object(cube);

	auto* sphere = ObjectManager::create_object("sphere");
	sphere->add_component<Model>();
	auto* md4 = sphere->get_component<Model>();
	auto* trans4 = sphere->get_component<Transform>();
	md4->set_meshes(AssetManager::get_meshes("sphere"));
	md4->color.set(0, 0, 1, 1);
	trans4->position.set(3.f, 0.f, 0.f);
	trans4->scale.set(1.f, 1.f, 1.f);
	register_object(sphere);
}

jeEnd
