#include "level1.h"
#include "game_logics.h"
#include "pch.h"

#include "components.hpp"

#include "colors.hpp"
#include <iostream>

jeBegin

void Level1::initialize()
{
	init_basic();
	init_sprite();

	// set screen color
	background.set(0.1f, 0.1f, 0.1f, 1.f);

	// base init
	Scene::initialize();
}

void Level1::update(float dt)
{
	/*if (renderer_2d)
	{
		DebugRenderer* dr = renderer_2d->get_component<DebugRenderer>();
		if (InputHandler::key_triggered(MOUSE_LEFT))
		{
			if (dr->picked())
				std::cout << "in\n";
			else
				std::cout << "out\n";
		}
	}*/

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

void Level1::init_sprite()
{
	// testing 2d renderer 
	renderer_2d = ObjectManager::create_object("renderer_2d");
	renderer_2d->add_component<Sprite>();
	// renderer_2d->add_component<DebugRenderer>();
	renderer_2d->add_component<TopDownController>();
	auto* renderer = renderer_2d->get_component<Sprite>();
	auto* animation = renderer_2d->get_component<Animation2D>();
	auto* trans = renderer_2d->get_component<Transform>();
	// renderer->set_texture(AssetManager::get_texture("rect"));
	renderer->set_texture(AssetManager::get_texture("testAnimation"));
	animation->activate(true);
	animation->set_frame(8);
	animation->set_speed(10.f);
	// animation->fix_frame(0);
	trans->scale.set(25, 40, 0.f);
	trans->position.z = 0.f;
	register_object(renderer_2d);

	//// gen rand obj
	//rand_obj = ObjectManager::create_object("rand_obj");
	//rand_obj->add_component<Sprite>();
	//trans = rand_obj->get_component<Transform>();
	//renderer = rand_obj->get_component<Sprite>();
	//renderer->set_texture(AssetManager::get_texture("rect"));
	//renderer->color.set_one();
	//trans->scale.set(100., 190., 0.f);
	//trans->position.set_zero();
	//register_object(rand_obj);

}

jeEnd
