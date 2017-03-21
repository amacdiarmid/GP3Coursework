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

private:
	ALCdevice* deviceAL;
	ALCcontext* contextAL;
};