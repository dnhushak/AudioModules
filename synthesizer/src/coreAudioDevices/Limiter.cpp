#include "Limiter.hpp"

namespace chip {
	
	Limiter::Limiter(int initBufferSize, int initSampleRate) {
		resizeBuffer(initBufferSize);
		changeSampleRate(initSampleRate);
		setThreshold(-3);
		maxNumAudioDevices = 1;
	}

	float * Limiter::advance(int numSamples) {
		if ((int) audioDeviceList->size() > 0) {
			buffer = audioDeviceList->front()->advance(numSamples);
			for (int i = 0; i < numSamples; i++) {
				if (buffer[i] < 0 && buffer[i] < thresholdLo) {
					buffer[i] = thresholdLo;
					printf("Lowthresh\n");
				} else if (buffer[i] > 0 && buffer[i] > thresholdHi) {
					buffer[i] = thresholdHi;
					printf("Hithresh\n");
				}
			}
		} else {
			zeroBuffer();
		}
		return buffer;
	}

	void Limiter::setThreshold(float newThreshold) {
		thresholdHi = dbToRatio(newThreshold);
		thresholdLo = -thresholdHi;
		printf("New threshold is %f\n", thresholdHi);
	}

}
