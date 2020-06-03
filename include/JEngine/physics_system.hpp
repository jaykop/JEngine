/******************************************************************************/
/*!
\file   physics_system.hpp
\author Jeong Juyong
\par    email: jaykop.jy\@gmail.com
\date   2019/06/15(yy/mm/dd)

\description
Contains the definition of PhysicsSystem class
*/
/******************************************************************************/

#pragma once
#include <macros.hpp>

jeBegin

class PhysicsSystem {

	// Prevent to clone this class
	PhysicsSystem() = delete;
	~PhysicsSystem() = delete;

	jePreventClone(PhysicsSystem)

	friend class Scene;

public:



private:

	static void initialize();
	static void update(float dt);
	static void close();

};

jeEnd