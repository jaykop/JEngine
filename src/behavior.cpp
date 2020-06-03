#include <behavior.hpp>
#include <behavior_system.hpp>

jeBegin

void Behavior::add_to_system()
{
	BehaviorSystem::add_behavior(this);
}

void Behavior::remove_from_system()
{
	BehaviorSystem::remove_behavior(this);
}

jeEnd