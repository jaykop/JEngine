#include "level5.h"
#include "scene_controller.h"
#include "camera_controller.h"
#include "pch.h"
#include "light.hpp"
#include "model.hpp"
#include "animation_2d.hpp"
#include "sprite.hpp"
#include "debug_renderer.hpp"

jeBegin

const int NUMBER_OF_SPHERES = 2;
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

	GraphicSystem::grid.color.set(1.f, 0.f, 0.f);
}

void Level5::update(float dt)
{
	/*Transform* trans = cube->get_component<Transform>();
	if (InputHandler::key_pressed(KEY::LEFT))
	{
		trans->position += vec3(-1, 0, 0) * 3 * dt;
	}
	if (InputHandler::key_pressed(KEY::RIGHT))
	{
		trans->position += vec3(1, 0, 0) * 3 * dt;
	}
	if (InputHandler::key_pressed(KEY::UP))
	{
		trans->position += vec3(0, 1, 0) * 3 * dt;
	}
	if (InputHandler::key_pressed(KEY::DOWN))
	{
		trans->position += vec3(0, -1, 0) * 3 * dt;
	}*/
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
	Object* cube = ObjectManager::create_object("cube");
	cube->add_component<Model>();
	auto* md3 = cube->get_component<Model>();
	auto* trans3 = cube->get_component<Transform>();
	md3->set_meshes(AssetManager::get_meshes("cube"));
	md3->shadow = true;
	md3->color.set(1, 0, 0, 1);
	trans3->position.set(-5.f, 0.f, 0.f);
	trans3->set_euler_deg(0.f, 0.f, 0.f);
	trans3->scale.set(1.f, 1.f, 1.f);
	register_object(cube);

	auto* sphere = ObjectManager::create_object("sphere");
	sphere->add_component<Model>();
	auto* md4 = sphere->get_component<Model>();
	auto* trans4 = sphere->get_component<Transform>();
	md4->set_meshes(AssetManager::get_meshes("sphere"));
	md4->color.set(0, 0, 1, 1);
	md4->shadow = true;
	trans4->position.set(5.f, 0.f, 0.f);
	trans3->set_euler_deg(0.f, 0.f, 0.f);
	trans4->scale.set(1.f, 1.f, 1.f);
	register_object(sphere);

	//auto* cube = ObjectManager::create_object("cube");
	//cube->add_component<Model>();
	//auto* md3 = cube->get_component<Model>();
	//auto* trans3 = cube->get_component<Transform>();
	//md3->set_meshes(AssetManager::get_meshes("cube"));
	//// md3->color.set(1, 0, 0, 1);
	//trans3->position.set(-3.f, 0.f, 0.f);
	//trans3->set_euler_deg(0.f, 0.f, 0.f);
	//trans3->scale.set(10.f, 10.f, .1f);
	//register_object(cube);

	auto* renderer_2d = ObjectManager::create_object("renderer_2d");
	renderer_2d->add_component<Sprite>();
	renderer_2d->add_component<DebugRenderer>();
	auto* renderer = renderer_2d->get_component<Sprite>();
	auto* animation = renderer_2d->get_component<Animation2D>();
	auto* trans = renderer_2d->get_component<Transform>();
	renderer->set_texture(AssetManager::get_texture("testAnimation"));
	renderer->prjType = Renderer::ProjectType::PERSPECTIVE;
	renderer->shadow = true;
	// renderer->status |= Renderer::IS_BILBOARD;
	animation->activate(true);
	animation->set_frame(8);
	animation->set_speed(10.f);
	// animation->fix_frame(0);
	trans->scale.set(2.5, 4., 0.f);
	trans->position.z = 0.f;
	register_object(renderer_2d);
}

jeEnd
