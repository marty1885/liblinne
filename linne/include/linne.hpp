#ifndef LIBLINNE_LIBLINNE_CPP
#define LIBLINNE_LIBLINNE_CPP

#include <AL/alc.h>
#include <AL/al.h>
#include <math.h>
#include <stdio.h>
#include <limits.h>

namespace Linne
{
#define SECOND 1
#define SAMPLING_HZ 44100
#define BUFFER_LENGTH (SECOND * SAMPLING_HZ)
#define SOUND_HZ 440.0

class PlaybackSystem
{
public:
	PlaybackSystem()
	{
		// Initialization
		ALshort* data = new ALshort[BUFFER_LENGTH*2];
		device = alcOpenDevice(NULL);
		context = alcCreateContext(device, NULL);
		alcMakeContextCurrent(context);
		alGenBuffers(1, &buffer);

		// Generate sine wave data
		for (int i=0; i<BUFFER_LENGTH;i++)
		{
			data[i*2] = sinf(2.0 * M_PI * SOUND_HZ * i / SAMPLING_HZ) * SHRT_MAX;
			data[i*2+1] = -1 * sinf(2.0 * M_PI * SOUND_HZ * i / SAMPLING_HZ) * SHRT_MAX; // antiphase
		}

		// Output looping sine wave
		alBufferData(buffer, AL_FORMAT_STEREO16, data, sizeof(ALshort)*BUFFER_LENGTH*2, SAMPLING_HZ);
		alGenSources(1, &source);
		alSourcei(source, AL_BUFFER, buffer);
		alSourcei(source, AL_LOOPING, AL_TRUE);
	}

	virtual ~PlaybackSystem()
	{
		// Finalization
		delete [] data;
		data = nullptr;
		alDeleteSources(1, &source);
		alDeleteBuffers(1, &buffer);
		alcMakeContextCurrent(NULL);
		alcDestroyContext(context);
		alcCloseDevice(device);
	}

	void play()
	{
		alSourcePlay(source);
		// Wait to exit
		printf("Press any key to exit.");
		getchar();
		alSourceStop(source);
	}
protected:
	ALCdevice *device;
	ALCcontext *context;
	ALshort* data = nullptr;
	ALuint buffer;
	ALuint source;

};

}

#endif
