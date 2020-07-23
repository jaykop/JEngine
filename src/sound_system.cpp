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
#include <debug_tools.hpp>
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
    ;
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

void SoundSystem::init_fmod()
{
    // create fmod system
    FMOD_RESULT result;
    result = FMOD::System_Create(&system_);
    DEBUG_ASSERT(FMOD_RESULT::FMOD_OK != result, "Failed to init FMOD");

    // check version
    unsigned int version;
    result = system_->getVersion(&version);
    DEBUG_ASSERT(FMOD_RESULT::FMOD_OK != result, "Version error of FMOD");
    if (version < FMOD_VERSION)
        jeDebugPrint("FMOD version error or FMOD lib version %08x doesn't match header version %08x", version, FMOD_VERSION);

    // init fmod system
    void* extradriverdata = nullptr;
    system_->init(32, FMOD_INIT_NORMAL, extradriverdata);
}

void SoundSystem::close_fmod()
{
    system_->close();
    system_->release();
}

jeEnd
