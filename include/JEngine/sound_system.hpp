/******************************************************************************/
/*!
\file   sound_system.hpp
\author Jeong Juyong
\par    email: jaykop.jy\@gmail.com
\date   2019/06/15(yy/mm/dd)

\description
Contains the definition of SoundSystem class
*/
/******************************************************************************/

#pragma once
#include <macros.hpp>

jeBegin

class SoundSystem {

	// Prevent to clone this class
	SoundSystem() = delete;
	~SoundSystem() = delete;

	jePreventClone(SoundSystem)

	friend class Scene;

public:



private:

	static void initialize();
	static void update(float dt);
	static void close();

};

jeEnd