#include "transform.hpp"
#include "camera.hpp"
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
		camera->set_pitch(0.f);
		camera->set_yaw(-90.f);
		camera->position.set(0.f, 0.f, 10.f);
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
	Camera* camera = owner_->get_component<Camera>();
	Transform* transform = owner_->get_component<Transform>();
	float offset = dt * speed;

	if (camera_2d)
	{
		if (InputHandler::key_pressed(KEY::W))
		{
			vec3 dis = vec3(0.f, 1.f, 0.f) * offset;
			transform->position += dis;
		}

		if (InputHandler::key_pressed(KEY::S))
		{
			vec3 dis = vec3(0.f, -1.f, 0.f) * offset;
			transform->position += dis;
		}
	}
	else
	{
		if (InputHandler::key_pressed(KEY::W))
		{
			transform->position += camera->get_front() * offset;
		}

		if (InputHandler::key_pressed(KEY::S))
		{
			transform->position -= camera->get_front() * offset;
		}
	}

	if (InputHandler::key_pressed(KEY::A))
	{
		transform->position -= camera->get_right() * offset;
	}

	if (InputHandler::key_pressed(KEY::D))
	{
		transform->position += camera->get_right() * offset;
	}

	camera->position = transform->position;
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

	last_dx = dx;
	last_dy = dy;

	dx = sensitivity * dt * std::fabsf(xoffset);
	dy = sensitivity * dt * std::fabsf(yoffset);
	if (dx > sensitivity) dx = last_dx;
	if (dy > sensitivity) dy = last_dy;

	if (xoffset > 0)
		camera->set_yaw(camera->get_yaw() + dx);
	else if (xoffset < 0)
		camera->set_yaw(camera->get_yaw() - dx);
	if (yoffset > 0)
		camera->set_pitch(camera->get_pitch() + dy);
	else if (yoffset < 0)
		camera->set_pitch(camera->get_pitch() - dy);

	if (camera->get_pitch() > 89.0f)
		camera->set_pitch(89.0f);
	else if (camera->get_pitch() < -89.0f)
		camera->set_pitch(-89.0f);
}

jeEnd