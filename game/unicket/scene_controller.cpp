#include <iostream>
#include "application.hpp"
#include "scene_controller.h"

#include "colors.hpp"

jeBegin

jeDefineUserComponentBuilder(SceneController);

SceneController::~SceneController()
{
	remove_from_system();
}

void SceneController::init() 
{ 
	GraphicSystem::grid.color = Color::blue;
}

void SceneController::update(float /*dt*/)
{
	if (InputHandler::key_pressed(KEY::MOUSE_LEFT))
		std::cout << "MOUSE_LEFT\n";

	if (InputHandler::key_triggered(KEY::MOUSE_RIGHT)) {
		
		// TODO: Need to get position
		// InputHandler::position
		std::cout << "MOUSE_RIGHT\n";
		ObjectManager::remove_object("emitter1");
	}

	if (InputHandler::key_pressed(KEY::ENTER))
		std::cout << "ENTER\n";

	if (InputHandler::key_triggered(KEY::SPACE))
		GraphicSystem::renderGrid = !GraphicSystem::renderGrid;
	
	if (InputHandler::key_triggered(KEY::ESC))
		Application::quit();
}

void SceneController::close() { }

jeEnd