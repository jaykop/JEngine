/******************************************************************************/
/*!
\file   behavior_system.hpp
\author Jeong Juyong
\par    email: jaykop.jy\@gmail.com
\date   2019/06/15(yy/mm/dd)

\description
Contains the definition of BehaviorSystem class
*/
/******************************************************************************/
#pragma once
#include <macros.hpp>
#include <vector>
#include <unordered_map>

jeBegin

class Object;
class Behavior;
class BehaviorSystem {

	// Prevent to clone this class
	BehaviorSystem() = delete;
	~BehaviorSystem() = delete;

	jePreventClone(BehaviorSystem)

	friend class Scene;

	using Behaviors = std::vector<Behavior*>;

public:

	static void add_behavior(Behavior* behavior);
	static void remove_behavior(Behavior* behavior);

private:

	static void initialize();
	static void update(float dt);
	static void close();

	static Behaviors behaviors_;
};

jeEnd