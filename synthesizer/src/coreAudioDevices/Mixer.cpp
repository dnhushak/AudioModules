#include "Mixer.hpp"
namespace synth {
	Mixer::Mixer(int initBufferSize, int initSampleRate) {
		//constructor
		// Initialize the audio buffers
		resizeBuffer(initBufferSize);
		changeSampleRate(initSampleRate);
	}

	float * Mixer::advance(int numSamples) {
		// Zero out the buffer
		zeroBuffer();
		if (audioDeviceList->size()) {
			audCallbackIter = audioDeviceList->begin();
			while (audCallbackIter != audioDeviceList->end()) {
				//TODO: Restrict maximum number of devices
				// Add each element into the mixdown buffer
				for (int j = 0; j < numSamples; j++) {
					// Sum each advanced IAudio to the master mixed vector
					buffer[j] += *((*audCallbackIter)->advance(1));
				}
				audCallbackIter++;
			}
		}
		//Pointer to the summed buffer
		return buffer;
	}

	// Sets the maximum number of devices
	void Mixer::setMaxNumAudioDevices(int newMax) {
		// Check if a valid maximum. (-1 indicates no max, 0 indicates no objects)
		if (newMax > -2) {
			maxNumAudioDevices = newMax;
		}

		// If our new maximum is less than our current number of devices, reduce the current device count
		if (maxNumAudioDevices > -1 && numAudioDevices > maxNumAudioDevices) {
			numAudioDevices = maxNumAudioDevices;
		}
	}
}
