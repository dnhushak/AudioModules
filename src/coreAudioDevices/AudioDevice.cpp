#include "AudioDevice.hpp"

namespace modules {
	AudioDevice::AudioDevice() {
		buffer = (sample_t *) malloc(sizeof(sample_t) * bufferSize);
		zeroBuffer();
	}

	void AudioDevice::cleanup(){
		return;
	}
	// Set every value in buffer to 0
	void AudioDevice::zeroBuffer() {
		memset(buffer, 0, sizeof(sample_t)*bufferSize);
	}

	// Resize the buffer of the audio device
	void AudioDevice::setBufferSize(int newSize) {
		if (newSize > 0) {
			bufferSize = newSize;
		}

	}

	// Return the buffer size
	int AudioDevice::getBufferSize() {
		return bufferSize;
	}

	// Change the sample rate of the device
	void AudioDevice::setSampleRate(int newRate) {
		if (newRate > 0) {
			sampleRate = newRate;
		}
	}

	// Return the audio sampling rate
	int AudioDevice::getSampleRate() {
		return sampleRate;
	}

	AudioDevice::~AudioDevice() {
		free(buffer);
	}
}
