#pragma once
#include <macros.hpp>

namespace FMOD
{
	class System;
};

jeBegin

class SoundManager
{
	jePreventClone(SoundManager);

	friend class Audio;
	friend class SoundSystem;

public:

	static void init_fmod();
	static void close_fmod();

private:

	static FMOD::System* system_;
};

jeEnd