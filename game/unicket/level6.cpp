#include "level6.h"
#include "scene_controller.h"
#include "camera_controller.h"
#include "pch.h"
#include "model.hpp"

jeBegin

void Level6::initialize()
{
	init_basic();
	// init_models();

	// base init
	Scene::initialize();

	// GraphicSystem::skybox.texture = AssetManager::get_texture("church");
	GraphicSystem::skybox.textures[0] = AssetManager::get_texture("badomen_front");
	GraphicSystem::skybox.textures[1] = AssetManager::get_texture("badomen_back");
	GraphicSystem::skybox.textures[2] = AssetManager::get_texture("badomen_right");
	GraphicSystem::skybox.textures[3] = AssetManager::get_texture("badomen_left");
	GraphicSystem::skybox.textures[4] = AssetManager::get_texture("badomen_top");
	GraphicSystem::skybox.textures[5] = AssetManager::get_texture("badomen_bottom");
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
	camera->add_component<CameraController>();
	camera->get_component<Camera>()->position = vec3(0.f, 0.f, 10.f);
	register_object(camera);

	GraphicSystem::set_camera(camera->get_component<Camera>());

	// scene controller
	Object* controller = ObjectManager::create_object("scene_controller");
	controller->add_component<SceneController>();
	register_object(controller);
}

void Level6::init_models()
{
	Object* cube = ObjectManager::create_object("cube");
	cube->add_component<Model>();
	auto* md3 = cube->get_component<Model>();
	auto* trans3 = cube->get_component<Transform>();
	md3->set_meshes(AssetManager::get_meshes("cube"));
	md3->status |= Renderer::IS_LIGHTEN;
	md3->color.set(1, 0, 0, 1);
	trans3->position.set(-5.f, 0.f, 0.f);
	trans3->set_euler_deg(0.f, 0.f, 0.f);
	trans3->scale.set(1.f, 1.f, 1.f);
	register_object(cube);

	auto* sphere = ObjectManager::create_object("sphere");
	sphere->add_component<Model>();
	auto* md4 = sphere->get_component<Model>();
	auto* trans4 = sphere->get_component<Transform>();
	md4->set_meshes(AssetManager::get_meshes("sphere"));
	md4->color.set(0, 0, 1, 1);
	md3->status |= Renderer::IS_LIGHTEN;
	trans4->position.set(5.f, 0.f, 0.f);
	trans3->set_euler_deg(0.f, 0.f, 0.f);
	trans4->scale.set(1.f, 1.f, 1.f);
	register_object(sphere);

	//auto* cube = ObjectManager::create_object("cube");
	//cube->add_component<Model>();
	//auto* md3 = cube->get_component<Model>();
	//auto* trans3 = cube->get_component<Transform>();
	//md3->set_meshes(AssetManager::get_meshes("cube"));
	//// md3->color.set(1, 0, 0, 1);
	//trans3->position.set(-3.f, 0.f, 0.f);
	//trans3->set_euler_deg(0.f, 0.f, 0.f);
	//trans3->scale.set(10.f, 10.f, .1f);
	//register_object(cube);
}

jeEnd
