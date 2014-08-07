#include "VCA.hpp"

namespace modules {

	VCA::VCA() {
		setMaxNumDevices(2);
		setGain(0);
	}

	sample_t * VCA::advance(int numSamples) {
		if (isEmpty()) {
			zeroBuffer();
		} else {
			// Copy over the buffer of the first module
			memcpy(buffer, front()->advance(numSamples),
					sizeof(sample_t) * numSamples);

			// If there are two connected devices, matrix multiply the two
			if (getNumDevices() == 2) {
				// Get the second connected device
				deviceIter = begin();
				deviceIter++;
				// Multiply every sample of the first buffer by the second buffer
				for (int i = 0; i < numSamples; i++) {
					buffer[i] *= (*(*deviceIter)->advance(1));
				}
			}

			//Multiply the final buffer by the VCA gain
			for (int i = 0; i < numSamples; i++) {
				buffer[i] *= gain;
			}
		}
		return buffer;
	}

	void VCA::setGain(float volume) {
		// Volume is coming in dbs
		// 0 dbs => gain of 1
		gain = dbToRatio(volume);
	}
}
