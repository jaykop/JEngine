#include "is_ui.h"
#include "graphic_system.hpp"
#include "camera.hpp"
#include "transform.hpp"

jeBegin

jeDefineUserComponentBuilder(IsUI);

IsUI::~IsUI()
{
	remove_from_system();
}

void IsUI::init() 
{ 
	initPos = (get_owner()->get_component<Transform>()->position);
}

void IsUI::update(float /*dt*/)
{
	// for ortho
	vec3& pos = (get_owner()->get_component<Transform>()->position);
	vec3& camPos = GraphicSystem::get_camera()->position;

	pos.x = initPos.x + camPos.x;
	pos.y = initPos.y + camPos.y;
}

void IsUI::close() { }

jeEnd