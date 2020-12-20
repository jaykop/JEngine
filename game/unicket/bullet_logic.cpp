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
}

void BulletLogic::update(float dt)
{
	Transform* trans = get_owner()->get_component<Transform>();
	trans->position += vel * dt * speed;
	if (vec3::distance_sq(startPos, trans->position) > 15000.f)
	{
		// remove this object
		ObjectManager::remove_object(get_owner());
	}
}

void BulletLogic::close() { }

jeEnd