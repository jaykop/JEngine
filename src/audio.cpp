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
	: Component(owner), m_channel(0), m_sound(0), m_system(system),
	m_volume(0.5f), m_pause(false), m_play(true)
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
void Audio::Play(void)
{
	if (m_play)
		m_system->playSound(FMOD_CHANNEL_FREE, m_sound, false, &m_channel);
}

/******************************************************************************/
/*!
\brief - Stop audio
*/
/******************************************************************************/
void Audio::Stop(void)
{
	m_channel->stop();
}

/******************************************************************************/
/*!
\brief - Pause audio
\param boolean - toggle
*/
/******************************************************************************/
void Audio::Pause(bool boolean)
{
	m_channel->setPaused(boolean);
}

/******************************************************************************/
/*!
\brief - Set Mute Toggle
\param boolean - toggle
*/
/******************************************************************************/
void Audio::SetMuteToggle(bool toggle)
{
	m_play = toggle;
}

/******************************************************************************/
/*!
\brief - Get Mute Toggle
\return m_play
*/
/******************************************************************************/
bool Audio::GetMuteToggle(void) const
{
	return m_play;
}

/******************************************************************************/
/*!
\brief - Get Volume
\return m_volume
*/
/******************************************************************************/
float Audio::GetVolume(void) const
{
	return m_volume;
}

/******************************************************************************/
/*!
\brief - Set Volume
\param volume
*/
/******************************************************************************/
void Audio::SetVolume(float volume)
{
	m_volume = volume;
}

jeEnd