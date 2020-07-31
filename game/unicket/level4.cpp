#include "level4.h"
#include "scene_controller.h"
#include "camera_controller.h"
#include "pch.h"
#include "model.hpp"

jeBegin

void Level4::initialize()
{
	init_basic();
	init_models();

	// base init
	Scene::initialize();

	GraphicSystem::skybox.scale = 10.f;
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
	/*auto* earth = ObjectManager::create_object("earth");
	earth->add_component<Model>();
	auto* md1 = earth->get_component<Model>();
	auto* trans1 = earth->get_component<Transform>();
	md1->set_meshes(AssetManager::get_meshes("earth"));
	trans1->position.set(5.f, 0.f, -10.f);
	trans1->scale.set(1.f, 1.f, 1.f);
	register_object(earth);

	auto* whale = ObjectManager::create_object("whale");
	whale->add_component<Model>();
	auto* md2 = whale->get_component<Model>();
	auto* trans2 = whale->get_component<Transform>();
	md2->set_meshes(AssetManager::get_meshes("whale"));
	trans2->position.set(-5.f, 0.f, -10.f);
	trans2->scale.set(1.f, 1.f, 1.f);
	register_object(whale);

	auto* cube = ObjectManager::create_object("cube");
	cube->add_component<Model>();
	auto* md3 = cube->get_component<Model>();
	auto* trans3 = cube->get_component<Transform>();
	md3->set_meshes(AssetManager::get_meshes("cube"));
	md3->color.set(1, 0, 0, 1);
	trans3->position.set(0.f, 5.f, -10.f);
	trans3->scale.set(1.f, 1.f, 1.f);
	register_object(cube);*/

	auto* sphere = ObjectManager::create_object("sphere");
	sphere->add_component<Model>();
	auto* md4 = sphere->get_component<Model>();
	auto* trans4 = sphere->get_component<Transform>();
	md4->set_meshes(AssetManager::get_meshes("cube"));
	md4->status |= Renderer::IS_REFLECTED;
	//md4->status |= Renderer::IS_REFRACTED;
	md4->color.set(0, 0, 1, 1);
	trans4->scale.set(1.f, 1.f, 1.f);
	register_object(sphere);
}

jeEnd
