#include "bullet_logic.h"
#include "emitter.hpp"

jeBegin

jeDefineUserComponentBuilder(BulletLogic);

BulletLogic::~BulletLogic()
{
	remove_from_system();
}

void BulletLogic::init() 
{ 
	startPos = get_owner()->get_component<Transform>()->position;
}

void BulletLogic::update(float dt)
{
	//vec3 currentPos = get_owner()->get_component<Transform>()->position;
	//if (vec3::distance_sq(startPos, currentPos) > 100.f)
	//{
	//	// remove this object
	//	ObjectManager::remove_object(get_owner());
	//}
}

void BulletLogic::close() { }

jeEnd