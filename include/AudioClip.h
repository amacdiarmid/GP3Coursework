#pragma once
#include "OpenAL.h"
#include "Common.h"

class AudioClip
{
public:
	AudioClip(ALuint Buffer);
	~AudioClip();

	void Play(float volume, vec3 position);
	void Stop();
	void setLooping(bool loop);
	void setLocation(vec3 pos);

	void Test();

private:
	ALenum  error;

	ALuint AudioBuffer;
	ALuint AudioSource;

};