#include "OpenAL.h"

OpenAL::OpenAL()
{
	//open AL
	alutInit(NULL, NULL);

	//get the current volume of the listener
	alGetListenerf(AL_GAIN, &maxVolume);
	curVolume = maxVolume;
}

OpenAL::~OpenAL()
{
	alutExit();
}

void OpenAL::setVolume(int tempVol)
{
	//set the new volume 
	alListenerf(AL_GAIN, tempVol);
	curVolume = tempVol;
}

void OpenAL::toggleMute()
{
	//if 0 set colume to the last current volume if not set it to mute.
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
	//build the path to the file
	string Path = ASSET_PATH + AUDIO_PATH + modelPath;

	if ((error = alGetError()) != AL_NO_ERROR)
	{
		cout << alGetString(error) << endl;
	}

	//load the file into a buffer 
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
	//set the location of the listener every frame
	ALfloat listenerPos[] = { position.x, position.y, position.z };
	alListenerfv(AL_POSITION, listenerPos);
	if ((error = alutGetError()) != AL_NO_ERROR)
	{
		cout << alutGetErrorString(error) << endl;
	}

	//move the locaton of all the audio in this list. 
	for each (AudioClip* audClip in movingAudio)
	{
		audClip->UpdateAudioPos();
	}
}
