#include <iostream>
#include "application.hpp"
#include "scene_controller.h"

#include "game_scenes.h"
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
		SceneManager::set_next_scene<Level1>();
	if (InputHandler::key_triggered(KEY::NUM_2))
		SceneManager::set_next_scene<Level2>();
	if (InputHandler::key_triggered(KEY::NUM_3))
		SceneManager::set_next_scene<Level3>();
	if (InputHandler::key_triggered(KEY::BACK))
	{
		if (SceneManager::is_paused())
			SceneManager::resume();
		else 
			SceneManager::pause<Pause>();
	}
	if (InputHandler::key_triggered(KEY::MOUSE_LEFT))
	{
		auto pos = InputHandler::get_position();
		std::cout << pos.x << " " << pos.y << " " << pos.z << "\n";
	}

	if (InputHandler::key_pressed(KEY::MOUSE_RIGHT))
		std::cout << "MOUSE_RIGHT\n";
	
	if (InputHandler::key_triggered(KEY::ENTER))
	{
		std::cout << "ENTER\n";
		AssetManager::generate_screenshot("resource/screenshot/");
	}

	if (InputHandler::key_triggered(KEY::SPACE))
		GraphicSystem::grid.render = !GraphicSystem::grid.render;
	
	if (InputHandler::key_triggered(KEY::ESC))
		Application::quit();
}

void SceneController::close() { }

jeEnd