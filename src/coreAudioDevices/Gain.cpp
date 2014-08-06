#include "Gain.hpp"

namespace modules {

	Gain::Gain() {
		setMaxNumDevices(1);
		// Default gain to -6 db
		setGain(-6);
	}

	sample_t * Gain::advance(int numSamples) {
		if (isEmpty()) {
			zeroBuffer();
		} else {
			memcpy(buffer, front()->advance(numSamples), sizeof(sample_t) * numSamples);
			for (int i = 0; i < numSamples; i++) {
				buffer[i] *= gain;
			}
		}
		return buffer;
	}

	void Gain::setGain(float volume) {
		// Volume is coming it in dbFs
		// 0 dbFs => gain of 1
		gain = dbToRatio(volume);
	}
}
