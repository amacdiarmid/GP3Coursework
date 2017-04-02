#pragma once

#include <al.h>
#include <alc.h>
#include <AL/alut.h>
#include <xram.h>
#include <Common.h>
#include "AudioClip.h"

class OpenAL
{
public:
	OpenAL();
	~OpenAL();

	ALCdevice* getDevice() { return deviceAL; };
	ALCcontext* getContext() { return contextAL; };
	void setVolume(int vol);
	void toggleMute();
	ALuint CreateBuffer(string modelPath);

	void updateAudio(vec3 position);
	
	void AddmovingAudio(AudioClip* Temp) { movingAudio.push_back(Temp); }

private:
	ALenum  error;

	ALCdevice* deviceAL;
	ALCcontext* contextAL;

	float curVolume;
	float maxVolume;

	vector<AudioClip*>movingAudio;
};