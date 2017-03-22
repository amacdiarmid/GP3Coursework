#pragma once
#include "OpenAL.h"
#include "Common.h"

class AudioClip
{
public:
	AudioClip();
	~AudioClip();

	void CreateBuffer(string modelPath);
	void Play();
	void Stop();
	void setLooping(bool loop);

	void Test();

private:
	ALenum  error;

	ALuint AudioBuffer;
	ALuint AudioSource;

};