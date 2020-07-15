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

	// testing 2d renderer 
	Object* renderer_2d = ObjectManager::create_object("renderer_2d");
	renderer_2d->add_component<Sprite>();
	renderer_2d->add_component<DebugRenderer>();
	renderer_2d->add_component<TopDownController>();
	auto* renderer = renderer_2d->get_component<Sprite>();
	auto* animation = renderer_2d->get_component<Animation2D>();
	auto* trans1 = renderer_2d->get_component<Transform>();
	// renderer->set_texture(AssetManager::get_texture("rect"));
	renderer->set_texture(AssetManager::get_texture("testAnimation"));
	animation->activate(true);
	animation->set_frame(8);
	animation->set_speed(10.f);
	// animation->fix_frame(0);
	trans1->scale.set(25, 40, 0.f);
	trans1->position.z = -10.f;
	register_object(renderer_2d);

	// testing text
	Object* text = ObjectManager::create_object("text");
	text->add_component<Text>();
	auto* text_component = text->get_component<Text>();
	auto* trans2 = text->get_component<Transform>();
	//text_component->set_font();
	text_component->set_text(L"JEngine에 오신 걸 환영합니다\n");
	trans2->position.set(20.f, 0.f, 0.f);
	trans2->scale.set(.1f, .1f, 0.f);
	register_object(text);

	// testing emitter
	Object* emitter = ObjectManager::create_object("emitter");
	emitter->add_component<Emitter>();
	auto* emi = emitter->get_component<Emitter>();
	auto* trans3 = emitter->get_component<Transform>();
	emi->set_texture(AssetManager::get_texture("rect"));
	emi->set_colors(vec3::one, vec3::zero);
	emi->active = true;
	emi->life = 1.f;
	emi->velocity.set(15.f, 15.f, 0.f);
	// emi->direction.set(0.f, 0.f, 0.f);
	emi->set_size(100);
	trans3->position.set(-50.f, 0.f, 0.f);
	trans3->scale.set(1.f, 1.f, 0.f);
	register_object(emitter);

	// set screen color
	background.set(0.1, 0.1, 0.1, 1);

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

jeEnd