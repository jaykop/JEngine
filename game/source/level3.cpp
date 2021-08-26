#include "level3.h"
#include "scene_controller.h"
#include "player_controller.h"
#include "player_attack.h"
#include "script_controller.h"
#include "pch.h"
#include "sprite.hpp"
#include "animation_2d.hpp"
#include "debug_renderer.hpp"
#include "collider_2d.hpp"
#include "rigidbody.hpp"
#include "emitter.hpp"
#include "colors.hpp"

#include <iostream>

jeBegin

void Level3::initialize()
{
	init_basic();
	init_sprite();
	//init_block();

	// base init
	Scene::initialize();
}

void Level3::update(float dt)
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

void Level3::close()
{
	// base close
	Scene::close();
}

void Level3::init_basic()
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

void Level3::init_sprite()
{
	// testing 2d renderer 
	player = ObjectManager::create_object("player");
	player->add_component<Sprite>();
	player->add_component<DebugRenderer>();
	player->add_component<PlayerController>();
	player->add_component<Collider2D>();
	player->add_component<RigidBody>();
	player->add_component<PlayerAttack>();

	auto* renderer = player->get_component<Sprite>();
	auto* animation = player->get_component<Animation2D>();
	auto* trans = player->get_component<Transform>();
	auto* rb = player->get_component<RigidBody>();
	renderer->set_texture(AssetManager::get_texture("arrow"));
	renderer->prjType = Renderer::ProjectType::PERSPECTIVE;
	// rb->isStatic = false;
	// renderer->status |= Renderer::IS_BILBOARD;

	/*animation->activate(true);
	animation->set_frame(8);
	animation->set_speed(10.f);*/

	// animation->fix_frame(0);
	trans->scale.set(10, 10, 0.f);
	trans->position.set(-10, 10, 0);
	register_object(player);
}

void Level3::init_block()
{
	// testing 2d renderer 
	block = ObjectManager::create_object("block");
	block->add_component<Sprite>();
	block->add_component<Collider2D>();
	block->add_component<RigidBody>();

	auto* renderer = block->get_component<Sprite>();
	auto* animation = block->get_component<Animation2D>();
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
