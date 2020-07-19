#include "level1.h"
#include "game_logics.h"
#include "pch.h"

// components
#include "emitter.hpp"
#include "text.hpp"
#include "camera.hpp"
#include "sprite.hpp"
#include "transform.hpp"
#include "animation_2d.hpp"
#include "debug_renderer.hpp"

#include "colors.hpp"
#include <iostream>

jeBegin

void Level1::load()
{
	// base load
	Scene::load();
}

void Level1::initialize()
{
	init_basic();
	init_sprite();
	init_text();
	init_emitters();

	// set screen color
	background.set(0.1f, 0.1f, 0.1f, 1.f);

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

void Level1::close()
{
	// base close
	Scene::close();
}

void Level1::unload()
{
	// base unload
	Scene::unload();
}

void Level1::init_basic()
{
	// add camera
	Object* mainCamera = ObjectManager::create_object("main_camera");
	mainCamera->add_component<Camera>();
	mainCamera->add_component<CameraController>();
	mainCamera->get_component<Camera>()->target = vec3::zero + vec3(0.f, 0.f, 0.f);
	mainCamera->get_component<Camera>()->position = vec3::zero + vec3(0.f, 0.f, 100.f);
	register_object(mainCamera);

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

void Level1::init_text()
{
	// testing text
	Object* text = ObjectManager::create_object("text");
	text->add_component<Text>();
	auto* text_component = text->get_component<Text>();
	auto* trans = text->get_component<Transform>();
	//text_component->set_font();
	text_component->set_text(L"JEngine에 오신 걸 환영합니다\n");
	trans->position.set(20.f, 0.f, 0.f);
	trans->scale.set(.1f, .1f, 0.f);
	register_object(text);
}

void Level1::init_emitters()
{
	// testing emitter
	emitter1 = ObjectManager::create_object("emitter1");
	emitter1->add_component<Emitter>();
	auto* emi1 = emitter1->get_component<Emitter>();
	auto* trans1 = emitter1->get_component<Transform>();
	trans1->position.set(-50.f, 0.f, 0.f);
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
	trans2->position.set(-50.f, 25.f, 0.f);
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
	trans3->position.set(-50.f, -25.f, 0.f);
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
