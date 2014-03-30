#include "AudioDevice.hpp"

namespace synth {
	AudioDevice::AudioDevice(){
		sampleRate = 0;
		bufferSize = 256;
		buffer = (float *) malloc(sizeof(float[bufferSize]));
		resizeBuffer(bufferSize);
	}

// Default behavior for audio device, just returns 0
	float * AudioDevice::advance(int numSamples) {
		return buffer;
	}

// Perform cleanup on all devices in audio device list
	void AudioDevice::cleanup() {
		return;
	}

// Resize the buffer of the audio device
// DO NOT CALL DURING CALLBACK
	void AudioDevice::resizeBuffer(int newSize) {
		// Clears old buffer memory
		bufferSize = newSize;
		printf("Reallocating buffer....");
		buffer = (float*) realloc(buffer, sizeof(float[bufferSize]));
		zeroBuffer();
	}

// Change the sample rate of the device
	void AudioDevice::changeSampleRate(int newRate) {
		sampleRate = newRate;
	}

// Set every value in buffer to 0
	void AudioDevice::zeroBuffer() {
		for (int i = 0; i < bufferSize; i++) {
			buffer[i] = 0.0;
		}
	}

// Return the buffer size
	int AudioDevice::getBufferSize() {
		return bufferSize;
	}

// Return the audio sampling rate
	int AudioDevice::getSampleRate() {
		return sampleRate;
	}

	void cleanup() {
		return;
	}

	AudioDevice::~AudioDevice() {
		free(buffer);
	}
}
