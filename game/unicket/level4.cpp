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
	init_models();

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
	auto* md1 = model1->get_component<Model>();
	auto* trans1 = model1->get_component<Transform>();
	md1->set_meshes(AssetManager::get_meshes("cube"));
	trans1->position.set(5.f, 0.f, -10.f);
	trans1->scale.set(1.f, 1.f, 1.f);
	register_object(model1);

	model2 = ObjectManager::create_object("sphere");
	model2->add_component<Model>();
	auto* md2 = model2->get_component<Model>();
	auto* trans2 = model2->get_component<Transform>();
	md2->set_meshes(AssetManager::get_meshes("sphere"));
	trans2->position.set(-5.f, 0.f, -10.f);
	trans2->scale.set(1.f, 1.f, 1.f);
	register_object(model2);


}

jeEnd
