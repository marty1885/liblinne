#include <linne.hpp>

#include <iostream>
#include <vector>


int main()
{
	Linne::AudioSystem audioSystem;
	Linne::SoundBuffer buffer;
	float* audio = new float[44100*2];
	for(int i=0;i<44100;i++)
	{
		audio[i*2+0] = sinf(2.0 * M_PI * SOUND_HZ * i / SAMPLING_HZ);
		audio[i*2+1] = sinf(2.0 * M_PI * SOUND_HZ * i / SAMPLING_HZ);
	}
	buffer.loadFromMemory(audio,44100,2,44100);
	audioSystem.play();

	printf("Press any key to exit.");
	getchar();

	audioSystem.stop();

	delete [] audio;
	return 0;
}
