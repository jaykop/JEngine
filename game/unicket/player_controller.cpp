#include <iostream>
#include "transform.hpp"
#include "application.hpp"
#include "player_controller.h"
#include "rigidbody.hpp"

jeBegin

jeDefineUserComponentBuilder(PlayerController);

PlayerController::~PlayerController()
{
	remove_from_system();
}

void PlayerController::init() 
{ 
	moving_ = false;
	body_ = get_owner()->get_component<RigidBody>();
}

void PlayerController::update(float dt)
{
	Transform* trans =
		get_owner()->get_component<Transform>();

	static float offset = 0.f;

	// direction vector3
	static const vec3 LEFT(-1.f, 0.f, 0.f);
	static const vec3 RIGHT(1.f, 0.f, 0.f);
	static const vec3 UP(0.f, 1.f, 0.f);
	static const vec3 DOWN(0.f, -1.f, 0.f);

	// set obj's position
	if (moveByGrid_)
	{
		// can't modify the direction while obj moving
		if (!moving_)
		{
			if (InputHandler::key_pressed(KEY::LEFT))
			{
				moving_ = true;
				dist_ = LEFT * offset;
				nextPos_.x = currentPos_.x - 10.f;
			}
			if (InputHandler::key_pressed(KEY::RIGHT))
			{
				moving_ = true;
				dist_ = RIGHT * offset;
				nextPos_.x = currentPos_.x + 10.f;
			}
			if (InputHandler::key_pressed(KEY::UP))
			{
				moving_ = true;
				dist_ = UP * offset;
				nextPos_.y = currentPos_.y + 10.f;
			}
			if (InputHandler::key_pressed(KEY::DOWN))
			{
				moving_ = true;
				dist_ = DOWN * offset;
				nextPos_.y = currentPos_.y - 10.f;
			}
		}

		// reached the destination
		else
		{
			trans->position += dist_;

			float d = vec3::distance(nextPos_, trans->position);
			if (d < .9f)
			{
				trans->position = currentPos_ = nextPos_;
				moving_ = false;
			}
		}
	}

	else
	{
		// sliding motion
		if (InputHandler::key_triggered(KEY::SPACE))
		{
			sliding_ = true;
			offset = offset * 3.f; // boost speed
		}

		// while sliding on
		if (sliding_)
		{
			// decrease the speed
			offset -= dt;

			// once speed gets zero, off sliding motion
			if (offset < 0.f)
			{
				sliding_ = false;
			}
		}
		else
			offset = dt * speed;

		// movement
		if (InputHandler::key_pressed(KEY::A))
		{
			//body_->add_impulse(LEFT, dt);
			trans->position += LEFT * offset;
		}
		if (InputHandler::key_pressed(KEY::D))
		{
			//body_->add_impulse(RIGHT, dt);
			trans->position += RIGHT * offset;
		}
		if (InputHandler::key_pressed(KEY::W))
		{
			//body_->add_impulse(UP, dt);
			trans->position += UP * offset;
		}
		if (InputHandler::key_pressed(KEY::S))
		{
			//body_->add_impulse(DOWN, dt);
			trans->position += DOWN * offset;
		}
	}

	// set obj's rotation
	vec3 v3_deg = InputHandler::get_position().normalized();
	static const vec3 v3_horizon(1.f, 0.f, 0.f), z_axis(0, 0, 1.f);

	float dot = v3_horizon.x * v3_deg.x + v3_horizon.y * v3_deg.y; // dot product
	float det = v3_horizon.x * v3_deg.y - v3_horizon.y * v3_deg.x; // determinant
	float rad = atan2(det, dot);
	trans->set_euler_rad(z_axis * rad);

	// camera follows the character
	Camera* camera = GraphicSystem::get_camera();
	camera->position = trans->position;
	camera->position.z = 200.f;
}

void PlayerController::close() { }

jeEnd