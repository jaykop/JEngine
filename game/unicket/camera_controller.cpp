#include "gl_manager.hpp"
#include <iostream>
#include "transform.hpp"
#include "camera.hpp"
#include "application.hpp"
#include "camera_controller.h"

jeBegin

jeDefineUserComponentBuilder(CameraController);

CameraController::~CameraController()
{
	remove_from_system();
}

void CameraController::init() { }

void CameraController::update(float dt)
{
	Camera* camera = get_owner()->get_component<Camera>();

	if (InputHandler::key_triggered(KEY::MOUSE_RIGHT))
		camera_2d = !camera_2d;

	if (InputHandler::key_triggered(KEY::MOUSE_MIDDLE))
	{
		camera->set_pitch(Math::deg_to_rad(0.f));
		camera->set_yaw(Math::deg_to_rad(-90.f));
		camera->position.set(0.f, 0.f, 100.f);
		camera->zoom = 0.f;
	}

	if (!camera_2d)
		rotate(dt);
	
	move(dt);

	if (InputHandler::get_mouse_wheel_status(KEY::MOUSE_WHEEL_DOWN))
		camera->zoom += dt * 10.f;

	if (InputHandler::get_mouse_wheel_status(KEY::MOUSE_WHEEL_UP))
		camera->zoom -= dt * 10.f;
}

void CameraController::close() { }

void CameraController::move(float dt)
{
	Camera* camera = get_owner()->get_component<Camera>();
	float offset = dt * speed;

	if (camera_2d)
	{
		if (InputHandler::key_pressed(KEY::W))
		{
			vec3 dis = vec3(0.f, 1.f, 0.f) * offset;
			camera->position += dis;
		}

		if (InputHandler::key_pressed(KEY::S))
		{
			vec3 dis = vec3(0.f, -1.f, 0.f) * offset;
			camera->position += dis;
		}
	}
	else
	{
		if (InputHandler::key_pressed(KEY::W))
		{
			vec3 dis = vec3(0.f, 0.f, -1.f) * offset;
			camera->position += dis;
		}

		if (InputHandler::key_pressed(KEY::S))
		{
			vec3 dis = vec3(0.f, 0.f, 1.f) * offset;
			camera->position += dis;
		}
	}

	if (InputHandler::key_pressed(KEY::A))
	{
		vec3 dis = vec3(-1.f, 0.f, 0.f) * offset;
		camera->position += dis;
	}

	if (InputHandler::key_pressed(KEY::D))
	{
		vec3 dis = vec3(1.f, 0.f, 0.f) * offset;
		camera->position += dis;
	}
}

void CameraController::rotate(float dt)
{
	Camera* camera = get_owner()->get_component<Camera>();
	if (firstMouse)
	{
		lastX = InputHandler::get_position().x;
		lastY = InputHandler::get_position().y;
		firstMouse = false;
	}

	float xoffset = InputHandler::get_position().x - lastX;
	float yoffset = lastY - InputHandler::get_position().y;
	lastX = InputHandler::get_position().x;
	lastY = InputHandler::get_position().y;

	xoffset *= sensitivity * dt;
	yoffset *= sensitivity * dt;

	camera->set_yaw(camera->get_yaw() + xoffset);
	camera->set_pitch(camera->get_pitch() + yoffset);

	if (camera->get_pitch() > 89.0f)
		camera->set_pitch(Math::deg_to_rad(89.0f));
	else if (camera->get_pitch() < -89.0f)
		camera->set_pitch(Math::deg_to_rad(-89.0f));
}

jeEnd