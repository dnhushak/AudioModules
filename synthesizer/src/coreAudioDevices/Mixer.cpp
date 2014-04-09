#include "Mixer.hpp"
namespace synth {
	Mixer::Mixer() {
	}

	sample_t * Mixer::advance(int numSamples) {
		// Zero out the buffer
		zeroBuffer();
		if (!isEmpty()) {
			deviceIter = begin();
			while (deviceIter != end()) {
				// Add each element into the mixdown buffer
				for (int j = 0; j < numSamples; j++) {
					// Sum each advanced AudioDevice to the master mixed vector
					buffer[j] += *((*deviceIter)->advance(1));
				}
				deviceIter++;
			}
		}
		//Pointer to the summed buffer
		return buffer;
	}

}
