/******************************************************************************/
/*!
\file   Audio.cpp
\author Jeong Juyong
\par    email: jeykop14\@gmail.com
\date   2016/08/14(yy/mm/dd)
\description
Contains Audio class member and functions
All codes are written by Jaykop Jeong...
*/
/******************************************************************************/

#include <audio.hpp>
#include <sound_manager.hpp>
#include <fmod.hpp>

jeBegin

/******************************************************************************/
/*!
\brief - Audio Constructor
\param system - pointer to fmod main system
*/
/******************************************************************************/
Audio::Audio(Object* owner)
	: Component(owner), system_(SoundManager::system_), channel_(nullptr), sound_(nullptr),
	volume_(0.5f), pause_(false), play_(false)
{

}

/******************************************************************************/
/*!
\brief - Audio Destructor
*/
/******************************************************************************/
Audio::~Audio()
{

}

/******************************************************************************/
/*!
\brief - Play audio
*/
/******************************************************************************/
void Audio::play(void)
{
	if (!is_playing())
		system_->playSound(sound_, 0, false, &channel_);
}

/******************************************************************************/
/*!
\brief - Stop audio
*/
/******************************************************************************/
void Audio::stop(void)
{
	channel_->stop();
}

/******************************************************************************/
/*!
\brief - Pause audio
\param boolean - toggle
*/
/******************************************************************************/
void Audio::pause(bool boolean)
{
	channel_->setPaused(boolean);
}

/******************************************************************************/
/*!
\brief - Set Mute Toggle
\param boolean - toggle
*/
/******************************************************************************/
void Audio::mute(bool toggle)
{
	play_ = toggle;
}

/******************************************************************************/
/*!
\brief - Get Mute Toggle
\return m_play
*/
/******************************************************************************/
bool Audio::is_muted(void) const
{
	return play_;
}

/******************************************************************************/
/*!
\brief - Get Volume
\return m_volume
*/
/******************************************************************************/
float Audio::get_volume(void) const
{
	return volume_;
}

/******************************************************************************/
/*!
\brief - Set Volume
\param volume
*/
/******************************************************************************/
void Audio::set_volume(float volume)
{
	volume_ = volume;
}

bool Audio::is_playing() const
{
	bool playing;
	channel_->isPlaying(&playing);
	return playing;
}

bool Audio::is_paused() const
{
	bool paused;
	channel_->getPaused(&paused);
	return paused;
}

jeEnd