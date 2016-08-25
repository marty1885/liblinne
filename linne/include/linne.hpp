#ifndef LIBLINNE_LIBLINNE_CPP
#define LIBLINNE_LIBLINNE_CPP

#include <AL/alc.h>
#include <AL/al.h>

#include <math.h>
#include <stdio.h>
#include <limits.h>
#include <stdint.h>
#include <string.h>

namespace Linne
{
#define SECOND 1
#define SAMPLING_HZ 44100
#define BUFFER_LENGTH (SECOND * SAMPLING_HZ)
#define SOUND_HZ 440.0

class SoundBuffer
{
public:
	SoundBuffer()
	{
	}

	void loadFromMemory(const float* samples, size_t sampleCount, int channelCount, int sampleRate)
	{
		unload();
		update(channelCount,sampleRate);
		size_t size = sampleCount*channelCount;
		m_samples = new float[size];
		memcpy(m_samples,samples,size*sizeof(m_samples[0]));
	}

	virtual ~SoundBuffer()
	{
		unload();
	}

	void unload()
	{
		delete [] m_samples;
		m_samples = nullptr;
		m_sampleRate = 0;
		m_sampleCount = 0;
	}

	inline float* const getSamples() const
	{
		return m_samples;
	}

	size_t getSampleCount() const
	{
		return m_sampleCount;
	}

	int getSampleRate() const
	{
		return m_sampleRate;
	}

	int getChannelCount() const
	{
		return m_channelCount;
	}

	 SoundBuffer& operator =(const SoundBuffer& right)
	 {
		 loadFromMemory(right.getSamples(), right.getSampleCount(), right.getChannelCount(), right.getSampleRate());
		 return *this;
	 }
protected:
	void update(int channelCount, int sampleRate)
	{
		m_channelCount = channelCount;
		m_sampleRate = sampleRate;
	}

	size_t m_sampleCount = 0;
	float* m_samples = nullptr;
	int m_sampleRate = 0;
	int m_channelCount = 0;
};

class PlaybackSystem
{
public:
	PlaybackSystem()
	{
		// Initialization
		m_data = new ALshort[BUFFER_LENGTH*2];
		m_device = alcOpenDevice(NULL);
		m_context = alcCreateContext(m_device, NULL);
		alcMakeContextCurrent(m_context);

		// Generate sine wave data
		alGenBuffers(1, &m_buffer);
		for (int i=0; i<BUFFER_LENGTH;i++)
		{
			m_data[i*2] = sinf(2.0 * M_PI * SOUND_HZ * i / SAMPLING_HZ) * SHRT_MAX;
			m_data[i*2+1] = -1 * sinf(2.0 * M_PI * SOUND_HZ * i / SAMPLING_HZ) * SHRT_MAX; // antiphase
		}

		alBufferData(m_buffer, AL_FORMAT_STEREO16, m_data, sizeof(ALshort)*BUFFER_LENGTH*2, SAMPLING_HZ);
		alGenSources(1, &m_source);
		alSourcei(m_source, AL_BUFFER, m_buffer);
		alSourcei(m_source, AL_LOOPING, AL_FALSE);
	}

	virtual ~PlaybackSystem()
	{
		// Finalization
		delete [] m_data;
		m_data = nullptr;
		alDeleteSources(1, &m_source);
		alDeleteBuffers(1, &m_buffer);
		alcMakeContextCurrent(NULL);
		alcDestroyContext(m_context);
		alcCloseDevice(m_device);
	}

	void play()
	{
		// Wait to exit
		alSourcePlay(m_source);
		printf("Press any key to exit.");
		getchar();
		alSourceStop(m_source);
	}
protected:

	//OpenAL stuff
	ALCdevice *m_device;
	ALCcontext *m_context;
	ALshort* m_data = nullptr;
	ALuint m_buffer;
	ALuint m_source;

};

}

#endif
