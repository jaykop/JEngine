// necessary headers
#include "test_scene.h"
#include "game_logics.h"
#include "pch.h"

// components
#include "camera.hpp"
#include "sprite.hpp"
#include "transform.hpp"
#include "animation_2d.hpp"
#include "debug_renderer.hpp"

jeBegin

void TestScene::initialize()
{
	// add camera
	Object* mainCamera = ObjectManager::create_object("main_camera");
	mainCamera->add_component<Camera>();
	mainCamera->get_component<Camera>()->target_ = vec3::zero + vec3(0.f, 0.f, 0.f);
	mainCamera->get_component<Camera>()->position_ = vec3::zero + vec3(0.f, 0.f, 100.f);
	register_object(mainCamera);

	// scene controller
	Object* controller = ObjectManager::create_object("scene_controller");
	controller->add_component<SceneController>();
	register_object(controller);

	// testing 2d renderer 
	Object* renderer_2d = ObjectManager::create_object("renderer_2d");
	renderer_2d->add_component<Sprite>();
	renderer_2d->add_component<DebugRenderer>();
	renderer_2d->add_component<TopDownController>();

	auto* renderer = renderer_2d->get_component<Sprite>();
	// renderer->set_texture(AssetManager::get_texture("rect"));
	renderer->set_texture(AssetManager::get_texture("testAnimation"));

	auto* animation = renderer_2d->get_component<Animation2D>();
	animation->activate(true);
	animation->set_frame(8);
	animation->set_speed(10.f);
	// animation->fix_frame(0);

	auto* trans = renderer_2d->get_component<Transform>();
	trans->scale.set(25, 40, 0.f);
	register_object(renderer_2d);

	// set screen color
	background.set(0.1, 0.1, 0.1, 1);

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