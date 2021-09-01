#include "level1.h"
#include "scene_controller.h"
#include "player_controller.h"
#include "player_attack.h"
#include "script_controller.h"
#include "pch.h"
#include "debug_renderer.hpp"
#include "sprite.hpp"
#include "rigidbody.hpp"
#include "emitter.hpp"
#include "colors.hpp"
#include "environment_setter.h"

#include <iostream>

jeBegin

void Level1::initialize()
{
	init_basic();
	// init_player();
	// init_block();

	// base init
	Scene::initialize();
}

void Level1::update(float dt)
{
	// base update
	Scene::update(dt);
}

void Level1::close()
{
	// base close
	Scene::close();
}

void Level1::init_basic()
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

	// environment setting
	Object* walls = ObjectManager::create_object("walls");
	walls->add_component<EnvironmentSetter>();
	register_object(walls);

	GraphicSystem::get_camera()->position.z = 200.f;
}

void Level1::init_player()
{
	// testing 2d renderer 
	player = ObjectManager::create_object("player");
	player->add_component<Sprite>();
	player->add_component<DebugRenderer>();
	PlayerController* controller = player->add_component<PlayerController>();
	player->add_component<RigidBody>();
	// player->add_component<PlayerAttack>();

	auto* renderer = player->get_component<Sprite>();
	auto* trans = player->get_component<Transform>();
	auto* rb = player->get_component<RigidBody>();
	renderer->set_texture(AssetManager::get_texture("arrow"));
	renderer->prjType = Renderer::ProjectType::PERSPECTIVE;

	trans->scale.set(10, 10, 0.f);
	trans->position.set(0, 0, 0);
	register_object(player);

	Object* hitPointPlane = ObjectManager::create_object("plane");
	Sprite* plane = hitPointPlane->add_component<Sprite>();
	plane->set_texture(AssetManager::get_texture("rect"));
	plane->prjType = Renderer::ProjectType::PERSPECTIVE;
	plane->color.a = 0.5f;
	Transform* transform = hitPointPlane->get_component<Transform>();
	transform->scale.set(GraphicSystem::get_width(), GraphicSystem::get_height(), 0.f);
	transform->position.set(0,0,-1);
	register_object(hitPointPlane);

	controller->hitPointPlane = plane;
}

void Level1::init_block()
{
	// testing 2d renderer 
	block = ObjectManager::create_object("block");
	block->add_component<Sprite>();
	block->add_component<RigidBody>();

	auto* renderer = block->get_component<Sprite>();
	auto* trans = block->get_component<Transform>();
	auto* rb = block->get_component<RigidBody>();
	renderer->set_texture(AssetManager::get_texture("rect"));
	renderer->prjType = Renderer::ProjectType::PERSPECTIVE;
	rb->isStatic = true;

	trans->position.set(5, 5, 0.f);
	trans->scale.set(10, 10, 0.f);
	trans->position.z = 0.f;
	register_object(block);
}

jeEnd
