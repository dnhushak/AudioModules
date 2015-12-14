#include "Clipper.h"

namespace audio {
	
	Clipper::Clipper() {
		setThreshold(-3);
		setMaxNumDevices(1);
	}

	Clipper * Clipper::clone() {
		// Create new device
		Clipper * newDevice = new Clipper();
		// Set all member variables
		newDevice->state = this->state;
		newDevice->setThreshold(this->threshold);

		return newDevice;
	}

	void Clipper::alter(int paramNum, Parameter p) {
		// Check for parameter string
		switch (paramNum) {
			case 0:
				//Threshold
				setThreshold(p.getParam().f);
				break;
		}
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

	void Clipper::process(
			const sample_t *inBuffer,
			sample_t *outBuffer,
			int samplesToProcess,
			int numChannels) {

		// Account for multichannel buffers
		int totalSamples = samplesToProcess * numChannels;

		// Go through each sample in the buffer
		for (int i = 0; i < totalSamples; i++) {
			// If lower than low threshold...
			if (inBuffer[i] < thresholdLo) {
				// ... Set value to the low threshold
				outBuffer[i] = thresholdLo;
				// Else if higher than the high threshold...
			} else if (inBuffer[i] > thresholdHi) {
				// ... Set value to high threshold
				outBuffer[i] = thresholdHi;
			} else {
				outBuffer[i] = inBuffer[i];
			}
		}
	}

	float Clipper::getThreshold() {
		return threshold;
	}

	void Clipper::setThreshold(float newThreshold) {
		threshold = newThreshold;
		thresholdHi = (sample_t) (dbToRatio(newThreshold) * sampleMax);
		thresholdLo = -thresholdHi;
	}

}
