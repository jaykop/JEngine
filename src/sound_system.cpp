/******************************************************************************/
/*!
\file   sound_system.cpp
\author Jeong Juyong
\par    email: jaykop.jy\@gmail.com
\date   2019/06/15(yy/mm/dd)

\description
Contains the methods of SoundSystem class
*/
/******************************************************************************/

#include <sound_system.hpp>
#include <sound_manager.hpp>
#include <fmod.hpp>

jeBegin

FMOD::Channel* SoundSystem::channel_ = nullptr;
FMOD::Sound* SoundSystem::current_ = nullptr;
FMOD::System* SoundSystem::system_ = nullptr;

FMOD::Sound* SoundSystem::get_current_sound()
{
	if (channel_)
	{
		FMOD::Sound* current;
		channel_->getCurrentSound(&current);
		return current;
	}

	else
		return nullptr;
}

void SoundSystem::initialize()
{
	system_ = SoundManager::system_;
}

void SoundSystem::update(float /*dt*/)
{
	system_->update();

	if (channel_)
	{
		;
	}
}

void SoundSystem::close()
{
}

jeEnd
