// necessary headers
#include "test_scene.h"
#include "game_logics.h"
#include "pch.h"

// components
#include "camera.hpp"
#include "renderer.hpp"
#include "transform.hpp"

jeBegin

void TestScene::initialize()
{
	// add camera
	Object* mainCamera = ObjectManager::create_object("main_camera");
	mainCamera->add_component<Camera>();
	mainCamera->get_component<Camera>()->target_ = vec3::zero + vec3(0.f, 0.f, -1.f);
	mainCamera->get_component<Camera>()->position_ = vec3::zero;
	register_object(mainCamera);

	// scene controller
	Object* controller = ObjectManager::create_object("scene_controller");
	controller->add_component<SceneController>();
	register_object(controller);

	// testing 2d renderer 
	Object* renderer_2d = ObjectManager::create_object("renderer_2d");
	renderer_2d->add_component<Renderer>();
	renderer_2d->get_component<Renderer>();
	register_object(renderer_2d);

	// set screen color
	background.set(0.5, 0.5, 0.5, 1);

	// base init
	Scene::initialize();
}

void TestScene::update(float dt)
{
	// base update
	Scene::update(dt);
}

void TestScene::close()
{
	// base close
	Scene::close();
}

jeEnd