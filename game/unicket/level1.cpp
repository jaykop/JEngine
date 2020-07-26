#include "level1.h"
#include "scene_controller.h"
#include "camera_controller.h"
#include "topdown_controller.h"
#include "pch.h"
#include "sprite.hpp"
#include "animation_2d.hpp"
#include "debug_renderer.hpp"

#include <iostream>

jeBegin

void Level1::initialize()
{
	init_basic();
	init_sprite();
	init_ui();

	// base init
	Scene::initialize();
}

void Level1::update(float dt)
{
	if (renderer_2d)
	{
		DebugRenderer* dr = renderer_2d->get_component<DebugRenderer>();
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
	camera->add_component<CameraController>();
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
	renderer_2d->add_component<DebugRenderer>();
	renderer_2d->add_component<TopDownController>();
	auto* renderer = renderer_2d->get_component<Sprite>();
	auto* animation = renderer_2d->get_component<Animation2D>();
	auto* trans = renderer_2d->get_component<Transform>();
	renderer->set_texture(AssetManager::get_texture("testAnimation"));
	renderer->prjType = Renderer::ProjectType::PERSPECTIVE;
	// renderer->status |= Renderer::IS_BILBOARD;
	animation->activate(true);
	animation->set_frame(8);
	animation->set_speed(10.f);
	// animation->fix_frame(0);
	trans->scale.set(25, 40, 0.f);
	trans->position.z = 0.f;
	register_object(renderer_2d);


	auto child = ObjectManager::create_object("child");
	child->add_component<Sprite>();
	auto* rd1 = child->get_component<Sprite>();
	auto* trans1 = child->get_component<Transform>();
	rd1->set_texture(AssetManager::get_texture("rect"));
	rd1->prjType = Renderer::ProjectType::PERSPECTIVE;
	rd1->color.set(0.f, 0.f, 1.f, 1.f);
	trans1->scale.set(1, 1, 0.f);
	trans1->position.set(1, 1, 0.f);

	// add chidl before registration
	renderer_2d->add_child(child);
	register_object(child);
}

void Level1::init_ui()
{
	auto crosshair = ObjectManager::create_object("crosshair");
	crosshair->add_component<Sprite>();
	auto* rd1 = crosshair->get_component<Sprite>();
	auto* trans1 = crosshair->get_component<Transform>();
	rd1->set_texture(AssetManager::get_texture("crosshair"));
	rd1->prjType = Renderer::ProjectType::ORTHOGONAL;
	rd1->color.set(1.f, 1.f, 1.f, 1.f);
	//rd1->status |= Renderer::IS_BILBOARD;
	rd1->status |= Renderer::IS_FIXED;
	trans1->scale.set(25, 25, 0.f);
	trans1->position.set(0, 0, -100.f);
	register_object(crosshair);
}

jeEnd
