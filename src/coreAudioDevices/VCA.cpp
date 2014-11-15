#include "VCA.h"

namespace audio {

	VCA::VCA() {
		setMaxNumDevices(2);
		setGain(0);
	}

	sample_t * VCA::advance() {
		if (isEmpty()) {
			zeroBuffer();
		} else {
			copyToBuffer(front()->read(), bufferSize);

			// If there are two connected devices, matrix multiply the two
			if (getNumDevices() == 2) {
				// Multiply every sample of the first buffer by the second buffer, as well as the VCA gain
				for (int i = 0; i < bufferSize; i++) {
					buffer[i] *= back()->read()[i];
					buffer[i] *= gain;
				}
			}
			// If there is only one connected device, then VCA behaves just as the gain object
			else if (getNumDevices() == 1) {
				//Multiply the final buffer by the VCA gain
				for (int i = 0; i < bufferSize; i++) {
					buffer[i] *= gain;
				}
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
