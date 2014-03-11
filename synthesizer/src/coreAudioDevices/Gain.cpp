#include "Gain.hpp"

namespace chip {

	chip::Gain::Gain(int initBufferSize, int initSampleRate) {
		resizeBuffer(initBufferSize);
		changeSampleRate(initSampleRate);
		maxNumAudioDevices = 1;
		// Default gain to -6 db
		setGain(-6);
	}

	float * chip::Gain::advance(int numSamples) {
		if (numAudioDevices > 0) {
			buffer = audioDeviceList->at(0)->advance(numSamples);
			for (int i = 0; i < numSamples; i++) {
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
