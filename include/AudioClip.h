//for better code design this should have been an audioComponent
#pragma once
#include <al.h>
#include <alc.h>
#include <AL/alut.h>
#include "Common.h"
#include "GameObject.h"

class AudioClip
{
public:
	AudioClip(ALuint Buffer);
	~AudioClip();

	void Play(float volume, vec3 position);
	void Stop();
	void setLooping(bool loop);
	void setLocation(vec3 pos);
	void UpdateAudioPos();
	void SetOwner(GameObject* Tempowner) { owner = Tempowner; }
	void Test();

private:
	ALenum  error;

	ALuint AudioBuffer;
	ALuint AudioSource;
	GameObject* owner = NULL;
};