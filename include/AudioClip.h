#pragma once
#include "OpenAL.h"
#include "Common.h"

class AudioClip
{
public:
	AudioClip();
	~AudioClip();

	void CreateBuffer(string modelPath);

private:
	ALuint AudioBuffer;

};