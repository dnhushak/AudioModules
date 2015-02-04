#include "Mixer.h"
namespace audio {
	Mixer::Mixer() {
	}

	Mixer * Mixer::clone() {
		// Create new device
		Mixer * newDevice = new Mixer();
		// Set all member variables
		newDevice->state = this->state;

		return newDevice;
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

	void Mixer::process(const sample_t * *inBuffers, int numInBuffers,
			sample_t *outBuffer, int samplesToProcess, int numInChannels) {

		// Account for multichannel buffers
		int totalSamples = samplesToProcess * numInChannels;

		// If the first input buffer is not the same buffer as the output buffer...
		if (inBuffers[0] != outBuffer) {
			// ...Copy over the first input buffer to the output buffer
			for (int i = 0; i < totalSamples; i++) {
				outBuffer[i] = inBuffers[0][i];
			}
		}

		// For each additional input buffer, sample-wise add it to the output buffer
		for (int j = 1; j < numInBuffers; j++) {
			for (int i = 0; i < totalSamples; i++) {
				outBuffer[i] += inBuffers[j][i];
			}
		}
	}

}
