#include "Clipper.hpp"

namespace audio {
	
	Clipper::Clipper() {
		setThreshold(-3);
		setMaxNumDevices(1);
	}

	sample_t * Clipper::advance() {
		if (!isEmpty()) {

			// Get the buffer from the connected device
			copyToBuffer(front()->read(), bufferSize);

			// Go through each sample in the buffer
			for (int i = 0; i < bufferSize; i++) {
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

	void Clipper::setThreshold(float newThreshold) {
		thresholdHi = (sample_t) (dbToRatio(newThreshold) * sampleMax);
		thresholdLo = -thresholdHi;
	}

}
