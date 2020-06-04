#include <iostream>
#include "transform.hpp"
#include "application.hpp"
#include "topdown_controller.h"

jeBegin

jeDefineUserComponentBuilder(TopDownController);

void TopDownController::init() { }

void TopDownController::update(float dt)
{
	Transform* trans = 
		get_owner()->get_component<Transform>();

	float offset = dt * speed;

	if (InputHandler::key_pressed(KEY::LEFT)
		|| InputHandler::key_pressed(KEY::A))
		trans->position += vec3(-1.f, 0.f, 0.f) * offset;

	if (InputHandler::key_pressed(KEY::RIGHT)
		|| InputHandler::key_pressed(KEY::D))
		trans->position += vec3(1.f, 0.f, 0.f) * offset;

	if (InputHandler::key_pressed(KEY::UP)
		|| InputHandler::key_pressed(KEY::W))
		trans->position += vec3(0.f, 1.f, 0.f) * offset;

	if (InputHandler::key_pressed(KEY::DOWN)
		|| InputHandler::key_pressed(KEY::S))
		trans->position += vec3(0.f, -1.f, 0.f) * offset;
}

void TopDownController::close() { }

jeEnd