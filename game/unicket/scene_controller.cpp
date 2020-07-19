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
	// set grid attributes
	GraphicSystem::grid.color = Color::white;
	GraphicSystem::grid.prjType = Renderer::ProjectType::PERSPECTIVE;
}

void SceneController::update(float /*dt*/)
{
	if (InputHandler::key_triggered(KEY::NUM_0))
		SceneManager::restart();
	if (InputHandler::key_triggered(KEY::NUM_1))
		SceneManager::set_next_scene("level1");
	if (InputHandler::key_triggered(KEY::NUM_2))
		SceneManager::set_next_scene("level2");
	if (InputHandler::key_triggered(KEY::NUM_3))
		SceneManager::set_next_scene("level3");
	if (InputHandler::key_triggered(KEY::BACK))
		SceneManager::pause("pause");

	if (InputHandler::key_triggered(KEY::MOUSE_LEFT))
	{
		auto pos = InputHandler::get_position();
		std::cout << pos.x << " " << pos.y << " " << pos.z << "\n";
	}

	if (InputHandler::key_pressed(KEY::MOUSE_RIGHT))
		std::cout << "MOUSE_RIGHT\n";

	if (InputHandler::key_pressed(KEY::ENTER))
		std::cout << "ENTER\n";

	if (InputHandler::key_triggered(KEY::SPACE))
		GraphicSystem::renderGrid = !GraphicSystem::renderGrid;
	
	if (InputHandler::key_triggered(KEY::ESC))
		Application::quit();
}

void SceneController::close() { }

jeEnd