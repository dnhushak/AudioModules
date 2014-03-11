#include "Gain.hpp"

namespace chip {

	//TODO: fix gain adding of devices? Segfaulting??
	chip::Gain::Gain(int initBufferSize, int initSampleRate) {
		printf("1\n");
		resizeBuffer(initBufferSize);
		changeSampleRate(initSampleRate);
		//maxNumAudioDevices = 3;
		// Default gain to -6 db
		setGain(-6);

		printf("2\n");
	}

	float * chip::Gain::advance(int numSamples) {
		printf("gain advance\n");
		if (numAudioDevices > 0) {
			for (int i = 0; i < numSamples; i++) {
				buffer[i] = audioDeviceList->at(0)->advance(1)[0];
				buffer[i] *= gain;
			}
		} else {
			zeroBuffer();
		}
		return buffer;
	}

	void chip::Gain::setGain(float volume) {
		// Volume is coming it in dbFs
		// 0 dbFs => gain of 1
		gain = dbToRatio(volume);
	}
}
