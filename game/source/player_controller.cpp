#include <iostream>
#include "transform.hpp"
#include "application.hpp"
#include "player_controller.h"
#include "rigidbody.hpp"
#include "debug_renderer.hpp"
#include "sprite.hpp"

jeBegin

jeDefineUserComponentBuilder(PlayerController);

// direction vector3
static const vec3 D_LEFT(-1.f, 0.f, 0.f);
static const vec3 D_RIGHT(1.f, 0.f, 0.f);
static const vec3 D_UP(0.f, 1.f, 0.f);
static const vec3 D_DOWN(0.f, -1.f, 0.f);

PlayerController::~PlayerController()
{
	remove_from_system();
}

void PlayerController::init() 
{ 
	moving_ = false;
	body_ = owner_->get_component<RigidBody>();
	transform_ = owner_->get_component<Transform>();

	movementMode_ = MOVEMENT_MODE::IMPULSE;
	speed = 1000.f;
	// rotationMode_ = ;
}

void PlayerController::update(float dt)
{
	switch (movementMode_)
	{
	case MOVEMENT_MODE::GRID:
		move_gridbase();
		break;

	case MOVEMENT_MODE::IMPULSE:
		move_impulsebase(dt);
		break;

	case MOVEMENT_MODE::WORLD:
	default:
		move_worldbase(dt);
		break;
	}

	switch (rotationMode_)
	{
	case ROTATION_MODE::KEYBOARD:
		rotate_keyboardbase(dt);
		break;

	case ROTATION_MODE::MOUSE:
	default:
		rotate_mousebase(dt);
		break;
	}

	DebugRenderer* dr = owner_->get_component<DebugRenderer>();
	if (InputHandler::key_triggered(MOUSE_LEFT))
	{
		vec3 hitPoint;
		if (dr->picked(&hitPoint))
			std::cout << "in\n";
		else
			std::cout << "out\n";
		std::cout << hitPoint << "\n";
	}

	// fix_camera();
}

void PlayerController::close() { }

void PlayerController::rotate_mousebase(float dt)
{
	// set obj's rotation

	vec3 hitPoint;
	hitPointPlane->picked(&hitPoint);
	if (!hitPoint.length())
		return;
	hitPoint.z = 0;

	vec3 v3_deg = (hitPoint - transform_->position).normalized();
	static const vec3 v3_horizon(1.f, 0.f, 0.f), z_axis(0, 0, 1.f);

	float dot = v3_horizon.x * v3_deg.x + v3_horizon.y * v3_deg.y; // dot product
	float det = v3_horizon.x * v3_deg.y - v3_horizon.y * v3_deg.x; // determinant
	float rad = atan2(det, dot);
	transform_->set_euler_rad(z_axis * rad);

	if (InputHandler::key_triggered(KEY::MOUSE_LEFT))
	{
		body_->add_force(v3_deg * speed * 100, dt);
	}
}

void PlayerController::rotate_keyboardbase(float dt)
{
	float dtSpeed = dt * speed;

	if (InputHandler::key_pressed(KEY::Q))
	{
		float z = transform_->get_euler_deg().z + dtSpeed;
		if (z < 0.f) z += 360.f;
		else if (z >= 360.f) z -= 360.f;
		transform_->set_euler_deg(0.f, 0.f, z);
	}
	if (InputHandler::key_pressed(KEY::E))
	{
		float z = transform_->get_euler_deg().z - dtSpeed;
		if (z < 0.f) z += 360.f;
		else if (z >= 360.f) z -= 360.f;
		transform_->set_euler_deg(0.f, 0.f, z);
	}
}

void PlayerController::move_worldbase(float dt)
{
	float dtSpeed = dt * speed;

	// movement
	vec3 direction;
	if (InputHandler::key_pressed(KEY::A))
	{
		direction = D_LEFT * dtSpeed;
	}
	if (InputHandler::key_pressed(KEY::D))
	{
		direction = D_RIGHT * dtSpeed;
	}
	if (InputHandler::key_pressed(KEY::W))
	{
		direction = D_UP * dtSpeed;
	}
	if (InputHandler::key_pressed(KEY::S))
	{
		direction = D_DOWN * dtSpeed;
	}

	transform_->position.set(transform_->position + direction);
}

void PlayerController::move_gridbase()
{
	// can't modify the direction while obj moving
	if (!moving_)
	{
		if (InputHandler::key_pressed(KEY::LEFT))
		{
			moving_ = true;
			dist_ = D_LEFT * speed;
			nextPos_.x = currentPos_.x - 10.f;
		}
		if (InputHandler::key_pressed(KEY::RIGHT))
		{
			moving_ = true;
			dist_ = D_RIGHT * speed;
			nextPos_.x = currentPos_.x + 10.f;
		}
		if (InputHandler::key_pressed(KEY::UP))
		{
			moving_ = true;
			dist_ = D_UP * speed;
			nextPos_.y = currentPos_.y + 10.f;
		}
		if (InputHandler::key_pressed(KEY::DOWN))
		{
			moving_ = true;
			dist_ = D_DOWN * speed;
			nextPos_.y = currentPos_.y - 10.f;
		}
	}

	// reached the destination
	else
	{
		transform_->position += dist_;

		float d = vec3::distance(nextPos_, transform_->position);
		if (d < .9f)
		{
			transform_->position = currentPos_ = nextPos_;
			moving_ = false;
		}
	}
}

void PlayerController::move_impulsebase(float dt)
{
	float dtSpeed = dt * speed;

	// movement
	vec3 direction;
	if (InputHandler::key_pressed(KEY::A))
	{
		direction = D_LEFT * dtSpeed;
	}
	if (InputHandler::key_pressed(KEY::D))
	{
		direction = D_RIGHT * dtSpeed;
	}
	if (InputHandler::key_pressed(KEY::W))
	{
		direction = D_UP * dtSpeed;
	}
	if (InputHandler::key_pressed(KEY::S))
	{
		direction = D_DOWN * dtSpeed;
	}

	body_->add_force(direction * speed, dt);
}

void PlayerController::fix_camera()
{
	// camera follows the character
	Camera* camera = GraphicSystem::get_camera();
	camera->position = transform_->position;
	camera->position.z = 200.f;
}

jeEnd