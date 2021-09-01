#include "level2.h"
#include "scene_controller.h"
#include "player_controller.h"
#include "player_attack.h"
#include "script_controller.h"
#include "pch.h"
#include "sprite.hpp"
#include "animation_2d.hpp"
#include "debug_renderer.hpp"
#include "rigidbody.hpp"
#include "emitter.hpp"
#include "colors.hpp"

#include <iostream>

jeBegin

void Level2::initialize()
{
	init_basic();

	// base init
	Scene::initialize();
}

void Level2::update(float dt)
{
	if (scriptController)
	{
		Transform* transform = scriptController->get_component<Transform>();
		vec3 pos = GraphicSystem::get_camera()->position;
		pos.y = -GraphicSystem::get_height() * .35f;
		pos.z = 10.f;
		vec3 scale(GraphicSystem::get_width(), GraphicSystem::get_height() * .25f, 0);
		transform->position.set(pos);
		transform->scale.set(scale);
	}

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
	camera->get_component<Camera>()->position = vec3::zero + vec3(0.f, 0.f, 100.f);
	register_object(camera);

	GraphicSystem::set_camera(camera->get_component<Camera>());

	// scene controller
	Object* controller = ObjectManager::create_object("scene_controller");
	controller->add_component<SceneController>();
	register_object(controller);

	// script controller
	scriptController = ObjectManager::create_object("script_controller");
	scriptController->add_component<ScriptController>();
	//scriptController->add_component<Text>();
	auto* sprite = scriptController->add_component<Sprite>();
	auto* transform = scriptController->get_component<Transform>();
	transform->scale.set(10, 10, 1);
	// transform->scale.set_zero();
	sprite->set_texture(AssetManager::get_texture("grid"));
	sprite->prjType = Renderer::ProjectType::ORTHOGONAL;

	register_object(scriptController);
}

jeEnd
