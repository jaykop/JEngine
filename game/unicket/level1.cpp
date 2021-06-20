#include "level1.h"
#include "scene_controller.h"
#include "player_controller.h"
#include "player_attack.h"
#include "pch.h"
#include "sprite.hpp"
#include "animation_2d.hpp"
#include "debug_renderer.hpp"
#include "collider_2d.hpp"
#include "rigidbody.hpp"

#include <iostream>

jeBegin

void Level1::initialize()
{
	init_basic();
	init_sprite();
	init_block();

	// base init
	Scene::initialize();
}

void Level1::update(float dt)
{
	if (player)
	{
		DebugRenderer* dr = player->get_component<DebugRenderer>();
		if (InputHandler::key_triggered(MOUSE_LEFT))
		{
			if (dr->picked())
				std::cout << "in\n";
			else
				std::cout << "out\n";
		}
	}

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
}

void Level1::init_sprite()
{
	// testing 2d renderer 
	player = ObjectManager::create_object("player");
	player->add_component<Sprite>();
	player->add_component<DebugRenderer>();
	player->add_component<PlayerController>();
	player->add_component<Collider2D>();
	player->add_component<RigidBody>();
	// player->add_component<PlayerAttack>();

	auto* renderer = player->get_component<Sprite>();
	auto* animation = player->get_component<Animation2D>();
	auto* trans = player->get_component<Transform>();
	auto* rb = player->get_component<RigidBody>();
	renderer->set_texture(AssetManager::get_texture("arrow"));
	renderer->prjType = Renderer::ProjectType::PERSPECTIVE;
	rb->isStatic = false;
	// renderer->status |= Renderer::IS_BILBOARD;

	/*animation->activate(true);
	animation->set_frame(8);
	animation->set_speed(10.f);*/

	// animation->fix_frame(0);
	trans->scale.set(10, 10, 0.f);
	trans->position.z = 0.f;
	register_object(player);
}

void Level1::init_block()
{
	// testing 2d renderer 
	block = ObjectManager::create_object("block");
	block->add_component<Sprite>();
	block->add_component<Collider2D>();
	block->add_component<RigidBody>();

	auto* renderer = block->get_component<Sprite>();
	auto* animation = block->get_component<Animation2D>();
	auto* trans = block->get_component<Transform>();
	renderer->set_texture(AssetManager::get_texture("rect"));
	renderer->prjType = Renderer::ProjectType::PERSPECTIVE;

	trans->position.set(5, 5, 0.f);
	trans->scale.set(10, 10, 0.f);
	trans->position.z = 0.f;
	register_object(block);
}

jeEnd
