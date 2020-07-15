#include <iostream>
#include "transform.hpp"
#include "camera.hpp"
#include "application.hpp"
#include "camera_controller.h"

jeBegin

jeDefineUserComponentBuilder(CameraController);

void CameraController::init() { }

void CameraController::update(float dt)
{
	Camera* camera = get_owner()->get_component<Camera>();

	float offset = dt * speed;

	if (InputHandler::key_pressed(KEY::A))
	{
		vec3 dis = vec3(-1.f, 0.f, 0.f) * offset;
		camera->position += dis;
		camera->target += dis;
	}
	if (InputHandler::key_pressed(KEY::D))
	{
		vec3 dis = vec3(1.f, 0.f, 0.f) * offset;
		camera->position += dis;
		camera->target += dis;
	}
	if (InputHandler::key_pressed(KEY::W))
	{
		vec3 dis = vec3(0.f, 1.f, 0.f) * offset;
		camera->position += dis;
		camera->target += dis;
	}
	if (InputHandler::key_pressed(KEY::S))
	{
		vec3 dis = vec3(0.f, -1.f, 0.f) * offset;
		camera->position += dis;
		camera->target += dis;
	}
}

void CameraController::close() { }

jeEnd