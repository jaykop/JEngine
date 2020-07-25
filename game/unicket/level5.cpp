#include "level5.h"
#include "scene_controller.h"
#include "camera_controller.h"
#include "pch.h"
#include "light.hpp"
#include "model.hpp"

jeBegin

const int NUMBER_OF_SPHERES = 4;
const float DISTANCE_FROM_CENTER = 10.F;
const float DEGREE_INCREMENT = 360 / (NUMBER_OF_SPHERES);
const float ORBIT_SPEED = .5F;

void Level5::initialize()
{
	init_basic();
	init_lights();
	init_models();

	// base init
	Scene::initialize();
}

void Level5::update(float dt)
{
	for (int i = 0; i < NUMBER_OF_SPHERES; i++)
	{
		spherePos[i] += dt * ORBIT_SPEED;

		auto* trans = lights[i]->get_component<Transform>();
		trans->position.x = DISTANCE_FROM_CENTER * cos(spherePos[i]);
		trans->position.z = DISTANCE_FROM_CENTER * sin(spherePos[i]);
	}

	// base update
	Scene::update(dt);
}

void Level5::close()
{
	// base close
	Scene::close();
}

void Level5::init_basic()
{
	// add camera
	Object* camera = ObjectManager::create_object("main_camera");
	camera->add_component<Camera>();
	camera->add_component<CameraController>();
	camera->get_component<Camera>()->position = vec3(0.f, 0.f, 10.f);
	register_object(camera);

	GraphicSystem::set_camera(camera->get_component<Camera>());

	// scene controller
	Object* controller = ObjectManager::create_object("scene_controller");
	controller->add_component<SceneController>();
	register_object(controller);
}

void Level5::init_lights()
{
	spherePos.resize(NUMBER_OF_SPHERES);
	
	std::string name("light");

	for (int i = 0; i < NUMBER_OF_SPHERES; ++i) {

		// Set deg to initialize
		spherePos[i] = i * Math::deg_to_rad(DEGREE_INCREMENT);

		// light object
		std::string lightName(name + std::to_string(i));
		Object* obj = ObjectManager::create_object(lightName.c_str());
		obj->add_component<Light>();
		auto* trans = obj->get_component<Transform>();
		auto* light = obj->get_component<Light>();
		trans->scale.set(.3f, .3f, .3f);
		trans->position.x = DISTANCE_FROM_CENTER * cos(spherePos[i]);
		trans->position.z = DISTANCE_FROM_CENTER * sin(spherePos[i]);
		light->type = Light::LightType::POINT;
		register_object(obj);

		lights.emplace_back(obj);
	}
}

void Level5::init_models()
{
	auto* cube = ObjectManager::create_object("whale");
	cube->add_component<Model>();
	auto* md3 = cube->get_component<Model>();
	auto* trans3 = cube->get_component<Transform>();
	md3->set_meshes(AssetManager::get_meshes("whale"));
	// md3->color.set(1, 0, 0, 1);
	trans3->position.set(-3.f, 0.f, 0.f);
	trans3->set_euler_deg(0.f, 0.f, 0.f);
	trans3->scale.set(4.f, 4.f, 4.f);
	register_object(cube);

	auto* sphere = ObjectManager::create_object("bunny");
	sphere->add_component<Model>();
	auto* md4 = sphere->get_component<Model>();
	auto* trans4 = sphere->get_component<Transform>();
	md4->set_meshes(AssetManager::get_meshes("bunny"));
	md4->color.set(0, 0, 1, 1);
	trans4->position.set(3.f, 0.f, 0.f);
	trans3->set_euler_deg(0.f, 0.f, 0.f);
	trans4->scale.set(1.f, 1.f, 1.f);
	register_object(sphere);
}

jeEnd