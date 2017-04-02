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

ALuint OpenAL::CreateBuffer(string modelPath)
{
	string Path = ASSET_PATH + AUDIO_PATH + modelPath;

	if ((error = alGetError()) != AL_NO_ERROR)
	{
		cout << alGetString(error) << endl;
	}

	ALuint buffer;
	buffer = alutCreateBufferFromFile(Path.c_str());

	if ((error = alutGetError()) != AL_NO_ERROR)
	{
		cout << alutGetErrorString(error) << endl;
	}
	return buffer;
}

void OpenAL::updateAudio(vec3 position)
{
	ALfloat listenerPos[] = { position.x, position.y, position.z };
	alListenerfv(AL_POSITION, listenerPos);
	if ((error = alutGetError()) != AL_NO_ERROR)
	{
		cout << alutGetErrorString(error) << endl;
	}
}
