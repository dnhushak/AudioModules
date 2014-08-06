#include "Limiter.hpp"

namespace modules {
	
	Limiter::Limiter() {
		setThreshold(-3);
		setMaxNumDevices(1);
	}

	sample_t * Limiter::advance(int numSamples) {
		if (!isEmpty()) {

			// Get the buffer from the connected device
			buffer = front()->advance(numSamples);

			// Go through each sample in the buffer
			for (int i = 0; i < numSamples; i++) {
				// If lower than low threshold...
				if (buffer[i] < thresholdLo) {
					// ... Set value to the low threshold
					buffer[i] = thresholdLo;
				// Else if higher than the high threshold...
				} else if (buffer[i] > thresholdHi) {
					// ... Set value to high threshold
					buffer[i] = thresholdHi;
				}
			}
		} else {
			zeroBuffer();
		}
		return buffer;
	}

	void Limiter::setThreshold(float newThreshold) {
		thresholdHi = (sample_t) (dbToRatio(newThreshold) * sampleMax);
		thresholdLo = -thresholdHi;
	}

}
