#pragma once

#include <al.h>
#include <alc.h>
#include <AL/alut.h>
#include <xram.h>
#include <Common.h>

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

private:
	ALenum  error;

	ALCdevice* deviceAL;
	ALCcontext* contextAL;

	float curVolume;
	float maxVolume;
};