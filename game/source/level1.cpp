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
	//init_player();
	init_hitplane();

	for (int i = 0; i < 10; ++i)
		init_block();

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
	auto* renderer = player->add_component<Sprite>();
	player->add_component<DebugRenderer>();
	PlayerController* controller = player->add_component<PlayerController>();
	auto* rb = player->add_component<RigidBody>();
	rb->collisionType_ = RigidBody::ColliderType::RECT;
	// player->add_component<PlayerAttack>();

	auto* trans = player->get_component<Transform>();
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
	static int num = 0;
	// testing 2d renderer 
	block = ObjectManager::create_object(("block_"+ std::to_string(num++)).c_str());
	block->add_component<Sprite>();
	block->add_component<RigidBody>();
	PlayerController* controller = block->add_component<PlayerController>();
	controller->hitPointPlane = hitplane->get_component<Sprite>();

	auto* renderer = block->get_component<Sprite>();
	auto* trans = block->get_component<Transform>();
	auto* rb = block->get_component<RigidBody>();
	renderer->set_texture(AssetManager::get_texture("rect"));
	renderer->prjType = Renderer::ProjectType::PERSPECTIVE;
	rb->isStatic = false;
	rb->collisionType_ = RigidBody::ColliderType::RECT;
	rb->gravity = 9.8f;

	trans->position.set(Random::get_rand_float(-80, 80), Random::get_rand_float(-60, 60), 0.f);
	trans->scale.set(Random::get_rand_float(5, 10), Random::get_rand_float(5, 10), 0.f);
	trans->orientation = mat4::rotate_z(Math::deg_to_rad(Random::get_rand_float(Math::zero, Math::round)));
	trans->position.z = 0.f;
	register_object(block);
}

void Level1::init_hitplane()
{
	hitplane = ObjectManager::create_object("plane");
	Sprite* plane = hitplane->add_component<Sprite>();
	plane->set_texture(AssetManager::get_texture("rect"));
	plane->prjType = Renderer::ProjectType::PERSPECTIVE;
	plane->color.a = 0.5f;
	Transform* transform = hitplane->get_component<Transform>();
	transform->scale.set(GraphicSystem::get_width(), GraphicSystem::get_height(), 0.f);
	transform->position.set(0, 0, -1);
	register_object(hitplane);
}

jeEnd
