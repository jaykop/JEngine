/******************************************************************************/
/*!
\file   behavior_system.cpp
\author Jeong Juyong
\par    email: jaykop.jy\@gmail.com
\date   2019/06/15(yy/mm/dd)

\description
Contains the methods of BehaviorSystem class
*/
/******************************************************************************/
#include <behavior_system.hpp>
#include <behavior.hpp>
#include <object.hpp>

jeBegin

BehaviorSystem::Behaviors BehaviorSystem::behaviors_;

void BehaviorSystem::add_behavior(Behavior* behavior)
{
	behaviors_.push_back(behavior);
}

void BehaviorSystem::remove_behavior(Behavior* behavior)
{
	for (Behaviors::iterator it = behaviors_.begin();
		it != behaviors_.end(); ++it) {
		if ((*it)->get_owner()->get_id() == behavior->get_owner()->get_id()) {
			behaviors_.erase(it);
			break;
		}
	}
}

void BehaviorSystem::initialize()
{
	for (const auto& b : behaviors_)
		b->init();

}

void BehaviorSystem::update(float dt)
{
	for (const auto& b : behaviors_)
		b->update(dt);
}

void BehaviorSystem::close()
{
	for (const auto& b : behaviors_)
		b->close();

	behaviors_.clear();
}

jeEnd