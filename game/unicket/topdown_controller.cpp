#include <iostream>
#include "transform.hpp"
#include "application.hpp"
#include "topdown_controller.h"

jeBegin

jeDefineUserComponentBuilder(TopDownController);

TopDownController::~TopDownController()
{
	remove_from_system();
}

void TopDownController::init() { }

void TopDownController::update(float dt)
{
	Transform* trans =
		get_owner()->get_component<Transform>();

	float offset = dt * speed;

	if (InputHandler::key_pressed(KEY::LEFT))
	{
		vec3 dis = vec3(-1.f, 0.f, 0.f) * offset;
		trans->position += dis;
	}
	if (InputHandler::key_pressed(KEY::RIGHT))
	{
		vec3 dis = vec3(1.f, 0.f, 0.f) * offset;
		trans->position += dis;
	}
	if (InputHandler::key_pressed(KEY::UP))
	{
		vec3 dis = vec3(0.f, 1.f, 0.f) * offset;
		trans->position += dis;
	}
	if (InputHandler::key_pressed(KEY::DOWN))
	{
		vec3 dis = vec3(0.f, -1.f, 0.f) * offset;
		trans->position += dis;
	}
}

void TopDownController::close() { }

jeEnd