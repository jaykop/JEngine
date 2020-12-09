#include "application.hpp"
#include "scene_controller.h"
#include "game_scenes.h"
#include "colors.hpp"

#include <iostream>

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
	GraphicSystem::grid.divisions = 99;
	GraphicSystem::grid.scale = 990;
	GraphicSystem::grid.thickness = 0.05f;
}

void SceneController::update(float /*dt*/)
{
	if (InputHandler::key_triggered(KEY::TAB))
		Application::set_fullscreen(!Application::get_appdata().isFullscreen);

	if (InputHandler::key_triggered(KEY::NUM_0))
		SceneManager::restart();

	if (InputHandler::key_triggered(KEY::NUM_1))
	{
		if (SceneManager::is_paused())
			SceneManager::resume_and_next<Level1>();
		else
			SceneManager::set_next_scene<Level1>();
	}

	/*if (InputHandler::key_triggered(KEY::NUM_2))
	{
		if (SceneManager::is_paused())
			SceneManager::resume_and_next<Level2>();
		else
			SceneManager::set_next_scene<Level2>();
	}

	if (InputHandler::key_triggered(KEY::NUM_3))
	{
		if (SceneManager::is_paused())
			SceneManager::resume_and_next<Level3>();
		else
			SceneManager::set_next_scene<Level3>();
	}

	if (InputHandler::key_triggered(KEY::NUM_4))
	{
		if (SceneManager::is_paused())
			SceneManager::resume_and_next<Level4>();
		else
			SceneManager::set_next_scene<Level4>();
	}

	if (InputHandler::key_triggered(KEY::NUM_5))
	{
		if (SceneManager::is_paused())
			SceneManager::resume_and_next<Level5>();
		else
			SceneManager::set_next_scene<Level5>();
	}*/

	if (InputHandler::key_triggered(KEY::BACK))
	{
		if (SceneManager::is_paused())
			SceneManager::resume();
		else 
			SceneManager::pause<Pause>();
	}

	if (InputHandler::key_triggered(KEY::MOUSE_LEFT))
	{
		static bool hide = false;
		Application::hide_cursor(hide = !hide);
		auto pos = InputHandler::get_position();
		std::cout << pos.x << " " << pos.y << " " << pos.z << "\n";
	}
	
	if (InputHandler::key_triggered(KEY::F1))
		GraphicSystem::recompile_shaders();

	if (InputHandler::key_triggered(KEY::F12))
		AssetManager::generate_screenshot("resource/screenshot/");

	if (InputHandler::key_triggered(KEY::SPACE))
		GraphicSystem::grid.render = !GraphicSystem::grid.render;
	
	if (InputHandler::key_triggered(KEY::ESC))
		Application::quit();
}

void SceneController::close() { }

jeEnd