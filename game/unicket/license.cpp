#include "license.h"
#include "pch.h"
#include "sprite.hpp"
#include "level1.h"

jeBegin

void License::initialize()
{
	add_camera();
	add_fmod();
	add_rapidjson();
	add_freetype();

	rd = fmod->get_component<Sprite>();

	// base init
	Scene::initialize();
}

void License::update(float dt)
{
	if (fade)
	{
		rd->color.a -= speed * dt;
		if (rd->color.a < 0.f)
		{
			rd->color.a = 0.f;
			fade = false;
			++count;
		}
	}

	else
	{
		rd->color.a += speed * dt;
		if (rd->color.a > 1.f)
		{
			rd->color.a = 1.f;
			fade = true;
		}
	}

	switch (count)
	{
	case 1:
		// rapidjson
		rd = rj->get_component<Sprite>();
		break;
	case 2:
		// freetype
		rd = ft->get_component<Sprite>();
		break;
	case 3:
		// next scene
		SceneManager::set_next_scene<Level1>();
		break;
	}

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
	camera->get_component<Camera>()->position = vec3(0.f, 0.f, 100.f);

	register_object(camera);

	GraphicSystem::set_camera(camera->get_component<Camera>());
}

void License::add_fmod()
{
	fmod = ObjectManager::create_object("fmod");
	fmod->add_component<Sprite>();
	auto* renderer = fmod->get_component<Sprite>();
	auto* transform = fmod->get_component<Transform>();
	renderer->set_texture(AssetManager::get_texture("fmod"));
	renderer->color.a = 0.f;
	renderer->prjType = Renderer::ProjectType::ORTHOGONAL;
	transform->scale.set(728, 192, 0.f);

	register_object(fmod);
}

void License::add_rapidjson()
{
	rj = ObjectManager::create_object("rapidjson");
	rj->add_component<Sprite>();
	auto* renderer = rj->get_component<Sprite>();
	auto* transform = rj->get_component<Transform>();
	renderer->set_texture(AssetManager::get_texture("rapidjson"));
	renderer->color.a = 0.f;
	renderer->prjType = Renderer::ProjectType::ORTHOGONAL;
	transform->scale.set(250, 52, 0.f);

	register_object(rj);
}

void License::add_freetype()
{
	ft = ObjectManager::create_object("freetype");
	ft->add_component<Sprite>();
	auto* renderer = ft->get_component<Sprite>();
	auto* transform = ft->get_component<Transform>();
	renderer->set_texture(AssetManager::get_texture("freetype"));
	renderer->color.a = 0.f;
	renderer->prjType = Renderer::ProjectType::ORTHOGONAL;
	transform->scale.set(800, 134, 0.f);

	register_object(ft);
}

jeEnd
