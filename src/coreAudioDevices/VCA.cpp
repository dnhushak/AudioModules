#include "VCA.hpp"

namespace modules {

	VCA::VCA() {
		setMaxNumDevices(2);
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
				sample_t multBuffer = getDeviceByLoc(1)->advance(numSamples);
				for (int i = 0; i < numSamples; i++) {
					buffer[i] *= multBuffer[i];
				}
			}
		}
		return buffer;
	}
}
