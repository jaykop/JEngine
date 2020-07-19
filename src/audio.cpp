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

#include "audio.hpp"

jeBegin

/******************************************************************************/
/*!
\brief - Audio Constructor
\param system - pointer to fmod main system
*/
/******************************************************************************/
Audio::Audio(Object* owner)
	: Component(owner), channel_(nullptr), sound_(nullptr), system_(nullptr),
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
	//if (play_)
	//	system_->playSound(FMOD_CHANNEL_FREE, sound_, false, &channel_);
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

jeEnd