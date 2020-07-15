// necessary headers
#include "test_scene.h"
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

jeBegin

void TestScene::initialize()
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

void TestScene::update(float dt)
{
	// base update
	Scene::update(dt);
}

void TestScene::close()
{
	// base close
	Scene::close();
}

void TestScene::init_basic()
{
	// add camera
	Object* mainCamera = ObjectManager::create_object("main_camera");
	mainCamera->add_component<Camera>();
	mainCamera->get_component<Camera>()->target_ = vec3::zero + vec3(0.f, 0.f, 0.f);
	mainCamera->get_component<Camera>()->position_ = vec3::zero + vec3(0.f, 0.f, 100.f);
	register_object(mainCamera);

	// scene controller
	Object* controller = ObjectManager::create_object("scene_controller");
	controller->add_component<SceneController>();
	register_object(controller);
}

void TestScene::init_sprite()
{
	// testing 2d renderer 
	Object* renderer_2d = ObjectManager::create_object("renderer_2d");
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
	trans->position.z = -10.f;
	register_object(renderer_2d);
}

void TestScene::init_text()
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

void TestScene::init_emitters()
{
	// testing emitter
	Object* emitter1 = ObjectManager::create_object("emitter1");
	emitter1->add_component<Emitter>();
	auto* emi1 = emitter1->get_component<Emitter>();
	auto* trans1 = emitter1->get_component<Transform>();
	trans1->position.set(-50.f, 0.f, 0.f);
	trans1->scale.set(1.f, 1.f, 0.f);
	emi1->set_texture(AssetManager::get_texture("rect"));
	emi1->set_colors(vec3::one, vec3::zero);
	emi1->active = true;
	emi1->life = 1.f;
	emi1->velocity.set(15.f, 15.f, 0.f);
	// emi->direction.set(0.f, 0.f, 0.f);
	emi1->set_size(100);
	register_object(emitter1);

	Object* emitter2 = ObjectManager::create_object("emitter2");
	emitter2->add_component<Emitter>();
	emitter2->add_component<ExplodingParticleController>();
	auto* emi2 = emitter2->get_component<Emitter>();
	auto* trans2 = emitter2->get_component<Transform>();
	trans2->position.set(-50.f, 25.f, 0.f);
	trans2->scale.set(1.f, 1.f, 0.f);
	emi2->set_texture(AssetManager::get_texture("rect"));
	emi2->set_colors(vec3::one, vec3::zero);
	emi2->type = Emitter::ParticleType::EXPLOSION;
	emi2->active = true;
	emi2->life = .5f;
	emi2->velocity.set(30.f, 30.f, 0.f);
	// emi->direction.set(0.f, 0.f, 0.f);
	emi2->set_size(100);
	register_object(emitter2);
}

jeEnd