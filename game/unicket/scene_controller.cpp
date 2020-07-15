#include <iostream>
#include "application.hpp"
#include "scene_controller.h"

jeBegin

jeDefineUserComponentBuilder(SceneController);

void SceneController::init() { }

void SceneController::update(float /*dt*/)
{
	if (InputHandler::key_pressed(KEY::MOUSE_LEFT))
		std::cout << "MOUSE_LEFT\n";

	if (InputHandler::key_triggered(KEY::MOUSE_RIGHT)) {
		
		// TODO: Need to get position
		// InputHandler::position
		std::cout << "MOUSE_RIGHT\n";
	}

	if (InputHandler::get_mouse_wheel_status(KEY::MOUSE_WHEEL_DOWN))
		std::cout << "MOUSE_WHEEL_DOWN\n";

	if (InputHandler::get_mouse_wheel_status(KEY::MOUSE_WHEEL_UP))
		std::cout << "MOUSE_WHEEL_UP\n";

	if (InputHandler::key_pressed(KEY::ENTER))
		std::cout << "ENTER\n";

	if (InputHandler::key_triggered(KEY::SPACE))
		std::cout << "SPACE\n";

	if (InputHandler::key_triggered(KEY::ESC))
		Application::quit();
}

void SceneController::close() { }

jeEnd