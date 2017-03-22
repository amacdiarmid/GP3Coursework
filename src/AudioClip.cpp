#include "AudioClip.h"

AudioClip::AudioClip()
{

}

AudioClip::~AudioClip()
{

}

void AudioClip::CreateBuffer(string modelPath)
{
	string Path = ASSET_PATH + AUDIO_PATH + modelPath;

	if ((error = alGetError()) != AL_NO_ERROR)
	{
		cout << alGetString(error) << endl;
	}

	//alutLoadWAVFile("Spacial Harvest.wav", &m_OALFormat, (void **)&m_OALData, (ALsizei *)&m_OALBufferLen, &m_OALFrequency, &m_alLoop);
	AudioBuffer = alutCreateBufferFromFile(Path.c_str());
	//AudioBuffer = alutCreateBufferHelloWorld();
	if ((error = alutGetError()) != AL_NO_ERROR)
	{
		cout << alutGetErrorString(error) << endl;
	}

	alGenSources(1, &AudioSource);
	if ((error = alutGetError()) != AL_NO_ERROR)
	{
		cout << alutGetErrorString(error) << endl;
	}

	alSourcei(AudioSource, AL_BUFFER, AudioBuffer);
	if ((error = alutGetError()) != AL_NO_ERROR)
	{
		cout << alutGetErrorString(error) << endl;
	}
	
}

void AudioClip::Play()
{
	alSourcePlay(AudioSource);

	if ((error = alGetError()) != AL_NO_ERROR)
	{
		cout << alGetString(error) << endl;
	}
}

void AudioClip::Stop()
{
	alSourceStop(AudioSource);
}

void AudioClip::setLooping(bool loop)
{
	if (loop)
		alSourcei(AudioSource, AL_LOOPING, AL_TRUE);
	else
		alSourcei(AudioSource, AL_LOOPING, AL_FALSE);

	if ((error = alGetError()) != AL_NO_ERROR)
	{
		cout << alGetString(error) << endl;
	}
}

void AudioClip::Test()
{
	ALuint helloBuffer, helloSource;

	helloBuffer = alutCreateBufferHelloWorld();

	alGenSources(1, &helloSource);

	alSourcei(helloSource, AL_BUFFER, helloBuffer);

	alSourcePlay(helloSource);

}
