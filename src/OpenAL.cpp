#include "OpenAL.h"
#include "Common.h"

OpenAL::OpenAL()
{
	ALenum  error;

	deviceAL = alcOpenDevice(NULL);
	if ((error = alGetError()) != AL_NO_ERROR)
	{
		cout << alGetString(error) << endl;
	}

	contextAL = alcCreateContext(deviceAL, NULL);
	alcMakeContextCurrent(contextAL);
	if ((error = alGetError()) != AL_NO_ERROR)
	{
		cout << alGetString(error) << endl;
	}

	alutInit(NULL, NULL);
	if ((error = alutGetError()) != AL_NO_ERROR)
	{
		cout << alutGetErrorString(error) << endl;
	}

}

OpenAL::~OpenAL()
{
	contextAL = alcGetCurrentContext();
	deviceAL = alcGetContextsDevice(contextAL);
	alcMakeContextCurrent(NULL);
	alcDestroyContext(contextAL);
	alcCloseDevice(deviceAL);
	alutExit();
}