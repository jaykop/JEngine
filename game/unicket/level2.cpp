#include "level2.h"
#include "game_logics.h"
#include "pch.h"

#include "components.hpp"

#include "colors.hpp"
#include <iostream>

jeBegin

void Level2::initialize()
{
	init_basic();
	init_text();

	// set screen color
	background.set(0.1f, 0.1f, 0.1f, 1.f);

	// base init
	Scene::initialize();
}

void Level2::update(float dt)
{
	// base update
	Scene::update(dt);
}

void Level2::close()
{
	// base close
	Scene::close();
}

void Level2::init_basic()
{
	// add camera
	Object* camera = ObjectManager::create_object("main_camera");
	camera->add_component<Camera>();
	camera->add_component<CameraController>();
	camera->get_component<Camera>()->target = vec3::zero + vec3(0.f, 0.f, 0.f);
	camera->get_component<Camera>()->position = vec3::zero + vec3(0.f, 0.f, 100.f);
	register_object(camera);

	GraphicSystem::set_camera(camera->get_component<Camera>());

	// scene controller
	Object* controller = ObjectManager::create_object("scene_controller");
	controller->add_component<SceneController>();
	register_object(controller);
}

void Level2::init_text()
{
	// testing text
	Object* text = ObjectManager::create_object("text");
	text->add_component<Text>();
	auto* text_component = text->get_component<Text>();
	auto* trans = text->get_component<Transform>();
	//text_component->set_font();
	text_component->set_text(L"JEngine에 오신 걸 환영합니다\n");
	trans->scale.set(.1f, .1f, 0.f);
	register_object(text);
}

jeEnd
