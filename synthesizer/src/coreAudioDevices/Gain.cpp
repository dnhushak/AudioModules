#include "Gain.hpp"

namespace chip {

	void chip::Gain::Gain(int initBufferSize, int initSampleRate) {
		bufferSize = initBufferSize;
		sampleRate = initSampleRate;
		buffer = new float[bufferSize];

		// Default gain to -6 db
		setGain(-6);
	}

	float * chip::Gain::advance(int numSamples) {
		if (getNumAudioDevices() > 0) {
			for (int i = 0; i < numSamples; i++) {
				buffer[i] = (*audioDeviceList)[0]->advance(1);
				buffer[i] *= gain;
			}
		}
		else{
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
