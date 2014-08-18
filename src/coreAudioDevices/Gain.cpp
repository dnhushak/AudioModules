#include "Gain.hpp"

namespace audio {

	Gain::Gain() {
		setMaxNumDevices(1);
		// Default gain to 0 db
		setGain(0);
	}

	sample_t * Gain::advance(int numSamples) {
		if (isEmpty()) {
			zeroBuffer();
		} else {
			memcpy(buffer, front()->advance(numSamples),
					sizeof(sample_t) * numSamples);
			for (int i = 0; i < numSamples; i++) {
				buffer[i] *= gain;
			}
		}
		return buffer;
	}

	void Gain::setGain(float volume) {
		// Volume is coming in dbs
		// 0 dbs => gain of 1
		gain = dbToRatio(volume);
	}
}
