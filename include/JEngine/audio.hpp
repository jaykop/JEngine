/******************************************************************************/
/*!
\file   Audio.h
\author Jeong Juyong
\par    email: jeykop14\@gmail.com
\date   2016/08/03(yy/mm/dd)
\description
Contains Audio class
All codes are written by Jaykop Jeong...
*/
/******************************************************************************/
#pragma once
#include <component_builder.hpp>
#include <component.hpp>

namespace FMOD
{
	class Channel;
	class Sound;
	class System;
}

jeBegin

class Object;
jeDeclareComponentBuilder(Transform);

//! Audio class
class Audio : public Component
{

	jeBaseFriends(Audio);

public:

	void play(void);
	void stop(void);
	void pause(bool boolean);

	//! Manage mute toggle
	bool is_muted(void) const;
	void mute(bool toggle);

	//! Manage volume
	float get_volume(void) const;
	void set_volume(float volume);

	bool is_playing() const;
	bool is_paused() const;

protected:

	virtual void add_to_system() {};
	virtual void remove_from_system() {};
	virtual void load(const rapidjson::Value& /*data*/) {};

private:

	Audio(Object* owner);
	virtual ~Audio();

	Audio& operator=(const Audio& rhs);

	FMOD::System* system_;
	FMOD::Channel* channel_;
	FMOD::Sound* sound_; 

	float volume_;
	bool  pause_, play_;

};

jeEnd