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
	//vec3 v1(-1.f, 0., 0.0);
	//vec3 v2(1.f, 0., 0.0);
	//vec3 v3(0.f, 1.f, 0.0);

	//vec3 a(1.f, 1., 0.0);
	//bool in = a.inside(v1, v2, v3);

	//InputHandler::ray_intersects_triangle();

	GraphicSystem::grid.color = Color::blue;
}

void SceneController::update(float /*dt*/)
{
	if (InputHandler::key_triggered(KEY::MOUSE_LEFT))
	{
		auto pos = InputHandler::get_position();
		std::cout << pos.x << " " << pos.y << " " << pos.z << "\n";
	}

	if (InputHandler::key_pressed(KEY::MOUSE_RIGHT))
	{	
		std::cout << "MOUSE_RIGHT\n";
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