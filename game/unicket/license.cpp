#include "license.h"
#include "game_logics.h"
#include "pch.h"

#include "components.hpp"
#include "colors.hpp"

jeBegin

void License::initialize()
{
	add_camera();
	//add_fmod();
	add_rapidjson();
	// add_freetype();

	// base init
	Scene::initialize();
}

void License::update(float dt)
{
	// base update
	Scene::update(dt);
}

void License::close()
{
	// base close
	Scene::close();
}

void License::add_camera()
{
	camera = ObjectManager::create_object("main_camera");
	camera->add_component<Camera>();
	camera->get_component<Camera>()->target = vec3::zero;
	camera->get_component<Camera>()->position = vec3(0.f, 0.f, 100.f);

	register_object(camera);
}

void License::add_fmod()
{
	fmod = ObjectManager::create_object("fmod");
	fmod->add_component<Sprite>();
	auto* renderer = fmod->get_component<Sprite>();
	auto* transform = fmod->get_component<Transform>();
	renderer->set_texture(AssetManager::get_texture("fmod"));
	transform->scale.set(40, 30, 0.f);

	register_object(fmod);
}

void License::add_rapidjson()
{
	rj = ObjectManager::create_object("rapidjson");
	rj->add_component<Sprite>();
	auto* renderer = rj->get_component<Sprite>();
	auto* transform = rj->get_component<Transform>();
	renderer->set_texture(AssetManager::get_texture("rapidjson"));
	transform->scale.set(50, 25, 0.f);

	register_object(rj);
}

void License::add_freetype()
{
	ft = ObjectManager::create_object("freetype");
	ft->add_component<Sprite>();
	auto* renderer = ft->get_component<Sprite>();
	auto* transform = ft->get_component<Transform>();
	renderer->set_texture(AssetManager::get_texture("freetype"));
	transform->scale.set(25, 40, 0.f);

	register_object(ft);
}

jeEnd
