#include "AudioClip.h"

AudioClip::AudioClip()
{

}

AudioClip::~AudioClip()
{

}

void AudioClip::CreateBuffer(string modelPath)
{
	ALenum  error;

	string Path = ASSET_PATH + AUDIO_PATH + modelPath;

	// Generate Buffers
	if ((error = alGetError()) != AL_NO_ERROR)
	{
		cout << alGetString(error) << endl;
	}

	//alutLoadWAVFile("Spacial Harvest.wav", &m_OALFormat, (void **)&m_OALData, (ALsizei *)&m_OALBufferLen, &m_OALFrequency, &m_alLoop);
	AudioBuffer = alutCreateBufferFromFile(Path.c_str());
	if ((error = alutGetError()) != AL_NO_ERROR)
	{
		cout << alutGetErrorString(error) << endl;
	}
	

	//test
	alSourcePlay(AudioBuffer);
	if ((error = alGetError()) != AL_NO_ERROR)
	{
		cout << alGetString(error) << endl;
	}
	alSourcei(AudioBuffer, AL_LOOPING, AL_TRUE);
	if ((error = alGetError()) != AL_NO_ERROR)
	{
		cout << alGetString(error) << endl;
	}
}
