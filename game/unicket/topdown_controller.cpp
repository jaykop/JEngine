#include "transform.hpp"
#include "application.hpp"
#include "topdown_controller.h"

jeBegin

jeDefineUserComponentBuilder(TopDownController);

TopDownController::~TopDownController()
{
	remove_from_system();
}

void TopDownController::init() 
{ 
	moving = false;
}

void TopDownController::update(float dt)
{
	Transform* trans =
		get_owner()->get_component<Transform>();

	float offset = dt * speed;

	if (!moving)
	{
		if (InputHandler::key_pressed(KEY::LEFT))
		{
			moving = true;
			dist = vec3(-1.f, 0.f, 0.f) * offset;
			nextPos.x = currentPos.x - 10.f;
		}
		if (InputHandler::key_pressed(KEY::RIGHT))
		{
			moving = true;
			dist = vec3(1.f, 0.f, 0.f) * offset;
			nextPos.x = currentPos.x + 10.f;
		}
		if (InputHandler::key_pressed(KEY::UP))
		{
			moving = true;
			dist = vec3(0.f, 1.f, 0.f) * offset;
			nextPos.y = currentPos.y + 10.f;
		}
		if (InputHandler::key_pressed(KEY::DOWN))
		{
			moving = true;
			dist = vec3(0.f, -1.f, 0.f) * offset;
			nextPos.y = currentPos.y - 10.f;
		}
	}

	else
	{
		trans->position += dist;

		float d = vec3::distance(nextPos, trans->position);
		if (d < .9f)
		{
			trans->position = currentPos = nextPos;
			moving = false;
		}
	}

	Camera* camera = GraphicSystem::get_camera();
	camera->position = trans->position;
	camera->position.z = 100.f;
}

void TopDownController::close() { }

jeEnd