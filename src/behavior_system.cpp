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

std::stack<BehaviorSystem::Behaviors> BehaviorSystem::componentStack_;
BehaviorSystem::Behaviors BehaviorSystem::behaviors_;

void BehaviorSystem::add_behavior(Behavior* behavior)
{
	behaviors_.push_back(behavior);
}

void BehaviorSystem::remove_behavior(Behavior* behavior)
{
	for (auto it = behaviors_.begin(); it != behaviors_.end(); ++it) {
		if (*it == behavior) {
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
	// flxeible size change
	for (unsigned i = 0; i < behaviors_.size(); ++i)
		behaviors_[i]->update(dt);
}

void BehaviorSystem::close()
{
	for (const auto& b : behaviors_)
		b->close();

	behaviors_.clear();
	behaviors_.shrink_to_fit();
}

void BehaviorSystem::pause()
{
	componentStack_.emplace(behaviors_);
	behaviors_.clear();
}

void BehaviorSystem::resume()
{
	if (!componentStack_.empty())
	{
		behaviors_ = componentStack_.top();
		componentStack_.pop();
	}
}

jeEnd