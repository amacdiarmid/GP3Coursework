#include "AudioClip.h"

AudioClip::AudioClip(ALuint Buffer)
{
	AudioBuffer = Buffer;
	if ((error = alGetError()) != AL_NO_ERROR)
	{
		cout << alGetString(error) << endl;
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

AudioClip::~AudioClip()
{
	Stop();
	alDeleteSources(1, &AudioSource);
}

void AudioClip::Play(float volume, vec3 position)
{
	if ((error = alGetError()) != AL_NO_ERROR)
	{
		cout << alGetString(error) << endl;
	}

	ALfloat listenerPos[] = { position.x, position.y, position.z };
	alSourcefv(AudioSource, AL_POSITION, listenerPos);
	if ((error = alGetError()) != AL_NO_ERROR)
	{
		cout << alGetString(error) << endl;
	}

	alSourcef(AudioSource, AL_GAIN, volume);
	if ((error = alGetError()) != AL_NO_ERROR)
	{
		cout << alGetString(error) << endl;
	}

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

void AudioClip::setLocation(vec3 pos)
{
	ALfloat listenerPos[] = { pos.x, pos.y, pos.z };
	alGetSourcefv(AudioSource, AL_POSITION, listenerPos);
	if ((error = alGetError()) != AL_NO_ERROR)
	{
		cout << alGetString(error) << endl;
	}
}

void AudioClip::UpdateAudioPos()
{
	if (owner != NULL)
	{
		setLocation(owner->getWorldPos());
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
