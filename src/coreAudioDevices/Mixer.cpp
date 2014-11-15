#include "Mixer.h"
namespace audio {
	Mixer::Mixer() {
	}

	sample_t * Mixer::advance() {
		if (!isEmpty()) {
			// Grab the first item in the list and copy its buffer over first
			copyToBuffer(front()->read(), bufferSize);

			deviceIter = begin();
			deviceIter++;
			// Start at the second item
			while (deviceIter != end()) {
				// Add each element into the mixdown buffer
				for (int j = 0; j < bufferSize; j++) {
					// Sum each advanced AudioDevice to the master mixed vector
					buffer[j] += (*deviceIter)->read()[j];
				}
				deviceIter++;
			}
		} else {
			// No connected devices; zero out the buffer
			zeroBuffer();
		}
		//Pointer to the summed buffer
		return buffer;
	}

}
