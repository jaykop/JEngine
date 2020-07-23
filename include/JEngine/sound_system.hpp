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

namespace FMOD
{
	class Channel;
	class Sound;
	class System;
};

jeBegin

class SoundSystem {

	// Prevent to clone this class
	SoundSystem() = delete;
	~SoundSystem() = delete;

	jePreventClone(SoundSystem)

	friend class Scene;
	friend class Audio;

public:

	static FMOD::Sound* get_current_sound();

private:

	static void initialize();
	static void update(float dt);
	static void close();

	static void init_fmod();
	static void close_fmod();

	static FMOD::System* system_;
	static FMOD::Channel* channel_;
	static FMOD::Sound* current_;

};

jeEnd