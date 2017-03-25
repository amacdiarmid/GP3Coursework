#pragma once

#include <al.h>
#include <alc.h>
#include <AL/alut.h>
#include <xram.h>

class OpenAL
{
public:
	OpenAL();
	~OpenAL();

	ALCdevice* getDevice() { return deviceAL; };
	ALCcontext* getContext() { return contextAL; };
	void setVolume(int vol);
	void toggleMute();

private:
	ALCdevice* deviceAL;
	ALCcontext* contextAL;

	float curVolume;
	float maxVolume;
};