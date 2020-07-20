#include "level3.h"
#include "game_logics.h"
#include "pch.h"

#include "components.hpp"

#include "colors.hpp"
#include <iostream>

jeBegin

void Level3::initialize()
{
	init_basic();
	init_emitters();

	// set screen color
	background.set(0.1f, 0.1f, 0.1f, 1.f);

	// base init
	Scene::initialize();
}

void Level3::update(float dt)
{

	if (emitter1)
	{
		auto* emi1 = emitter1->get_component<Emitter>();
		static float angle = 0.f;
		angle += 100.f * dt;
		emi1->angle.set(angle, angle + 180.f);
		if (angle >= 360.f) angle = 0.f;
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

void Level3::init_emitters()
{
	// testing emitter
	emitter1 = ObjectManager::create_object("emitter1");
	emitter1->add_component<Emitter>();
	auto* emi1 = emitter1->get_component<Emitter>();
	auto* trans1 = emitter1->get_component<Transform>();
	trans1->scale.set(1.f, 1.f, 0.f);
	emi1->set_texture(AssetManager::get_texture("rect"));
	emi1->set_colors(Color::yellow, Color::red);
	emi1->active = true;
	emi1->life = 1.f;
	emi1->colorSpeed = 3.f;
	emi1->velocity.set(15.f, 15.f, 0.f);
	emi1->angle.set(0.f, 180.f);
	emi1->set_size(1000);
	register_object(emitter1);

	Object* emitter2 = ObjectManager::create_object("emitter2");
	emitter2->add_component<Emitter>();
	emitter2->add_component<ExplodingParticleController>();
	auto* emi2 = emitter2->get_component<Emitter>();
	auto* trans2 = emitter2->get_component<Transform>();
	trans2->position.set(0.f, 25.f, 0.f);
	trans2->scale.set(1.f, 1.f, 0.f);
	emi2->set_texture(AssetManager::get_texture("rect"));
	emi2->set_colors(Color::white, Color::black);
	emi2->type = Emitter::ParticleType::EXPLOSION;
	emi2->life = .5f;
	emi2->colorSpeed = 1.f;
	emi2->velocity.set(15.f, 15.f, 0.f);
	emi2->angle.set(0.f, 360.f);
	emi2->set_size(1000);
	register_object(emitter2);

	Object* emitter3 = ObjectManager::create_object("emitter3");
	emitter3->add_component<Emitter>();
	auto* emi3 = emitter3->get_component<Emitter>();
	auto* trans3 = emitter3->get_component<Transform>();
	trans3->position.set(0.f, -25.f, 0.f);
	trans3->scale.set(1.f, 1.f, 0.f);
	emi3->set_texture(AssetManager::get_texture("rect"));
	emi3->set_colors(Color::white, Color::blue);
	emi3->type = Emitter::ParticleType::WIDE;
	emi3->life = 3.f;
	emi3->rotationSpeed = 100.f;
	emi3->range.set(20.f, 1.f, 0.f);
	emi3->velocity.set(0.f, 10.f, 0.f);
	emi3->angle.set(270.f, 270.f);
	emi3->set_size(1000);
	emi3->set_pointSize(1.f);
	register_object(emitter3);
}

jeEnd
