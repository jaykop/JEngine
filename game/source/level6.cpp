#include "level6.h"
#include "scene_controller.h"
#include "camera_controller.h"
#include "pch.h"
#include "model.hpp"
#include "application.hpp"

jeBegin

void Level6::initialize()
{
	init_basic();
	init_models();
	GraphicSystem::skybox.render = true;
	GraphicSystem::skybox.scale = 10000.f;
	Application::hide_cursor(true);

	// base init
	Scene::initialize();
}

void Level6::update(float dt)
{
	static float degree = 0.f;
	static float dist = 2.f;
	Obj_->get_component<Transform>()->position.x = dist * cos(degree);
	Obj_->get_component<Transform>()->position.z = dist * sin(degree);

	degree += dt;

	// base update
	Scene::update(dt);
}

void Level6::close()
{
	// base close
	Scene::close();
}

void Level6::init_basic()
{
	// add camera
	camera_ = ObjectManager::create_object("main_camera");
	camera_->add_component<Camera>();
	camera_->get_component<Camera>()->position = vec3(0.f, 0.f, 10.f);
	CameraController* cc = camera_->add_component<CameraController>();
	cc->camera_2d = false;
	register_object(camera_);

	GraphicSystem::set_camera(camera_->get_component<Camera>());

	Obj_ = ObjectManager::create_object("object");
	auto* md4 = Obj_->add_component<Model>();
	md4->set_meshes(AssetManager::get_meshes("cube"));
	Obj_->get_component<Transform>()->scale.set(.05f, .05f, .05f);
	register_object(Obj_);

	// scene controller
	Object* controller = ObjectManager::create_object("scene_controller");
	controller->add_component<SceneController>();
	register_object(controller);
}

void Level6::init_models()
{
	auto* sphere = ObjectManager::create_object("sphere");
	sphere->add_component<Model>();
	auto* md4 = sphere->get_component<Model>();
	auto* trans4 = sphere->get_component<Transform>();
	md4->set_meshes(AssetManager::get_meshes("sphere"));
	md4->status |= Renderer::IS_REFRACTED;
	// md4->status |= Renderer::IS_REFLECTED;
	md4->color.set(0, 0, 1, 1);
	trans4->scale.set(1.f, 1.f, 1.f);
	register_object(sphere);
}

jeEnd
