#include "level6.h"
#include "scene_controller.h"
#include "camera_controller.h"
#include "pch.h"
#include "model.hpp"
#include "application.hpp"

jeBegin

void Level6::initialize()
{
	init_basic();
	init_models();
	GraphicSystem::skybox.render = true;
	GraphicSystem::skybox.scale = 100.f;
	Application::hide_cursor(true);

	// base init
	Scene::initialize();
}

void Level6::update(float dt)
{
	// base update
	Scene::update(dt);
}

void Level6::close()
{
	// base close
	Scene::close();
}

void Level6::init_basic()
{
	// add camera
	Object* camera = ObjectManager::create_object("main_camera");
	camera->add_component<Camera>();
	camera->get_component<Camera>()->position = vec3(0.f, 0.f, 10.f);
	CameraController* cc = camera->add_component<CameraController>();
	cc->camera_2d = false;
	auto* md4 = camera->add_component<Model>();
	md4->set_meshes(AssetManager::get_meshes("cube"));
	register_object(camera);

	GraphicSystem::set_camera(camera->get_component<Camera>());

	// scene controller
	Object* controller = ObjectManager::create_object("scene_controller");
	controller->add_component<SceneController>();
	register_object(controller);
}

void Level6::init_models()
{
	auto* sphere = ObjectManager::create_object("sphere");
	sphere->add_component<Model>();
	auto* md4 = sphere->get_component<Model>();
	auto* trans4 = sphere->get_component<Transform>();
	md4->set_meshes(AssetManager::get_meshes("sphere"));
	// md4->status |= Renderer::IS_REFRACTED;
	md4->status |= Renderer::IS_REFLECTED;
	md4->color.set(0, 0, 1, 1);
	trans4->scale.set(1.f, 1.f, 1.f);
	register_object(sphere);
}

jeEnd
