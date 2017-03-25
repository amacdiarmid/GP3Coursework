#include "OpenAL.h"
#include "Common.h"

OpenAL::OpenAL()
{
	alutInit(NULL, NULL);

	alGetListenerf(AL_GAIN, &maxVolume);
	curVolume = maxVolume;
}

OpenAL::~OpenAL()
{
	/*contextAL = alcGetCurrentContext();
	deviceAL = alcGetContextsDevice(contextAL);
	alcMakeContextCurrent(NULL);
	alcDestroyContext(contextAL);
	alcCloseDevice(deviceAL);*/
	alutExit();
}

void OpenAL::setVolume(int tempVol)
{
	alListenerf(AL_GAIN, tempVol);
	curVolume = tempVol;
}

void OpenAL::toggleMute()
{
	if (curVolume == 0)
	{
		alListenerf(AL_GAIN, maxVolume);
		curVolume = maxVolume;
	}
	else
	{
		alListenerf(AL_GAIN, 0);
		curVolume = 0;
	}
}
