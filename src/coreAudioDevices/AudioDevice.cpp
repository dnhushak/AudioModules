#include "AudioDevice.hpp"

namespace audio {
	AudioDevice::AudioDevice() {
		buffer = (sample_t *) malloc(sizeof(sample_t) * bufferSize);
		zeroBuffer();
	}

	void AudioDevice::cleanup() {
		return;
	}

	sample_t * AudioDevice::read(){
		return buffer;
	}

	void AudioDevice::zeroBuffer() {
		memset(buffer, 0, sizeof(sample_t) * bufferSize);
	}

	void AudioDevice::setBufferSize(int newSize) {
		if (newSize > 0) {
			bufferSize = newSize;
			buffer = (sample_t *) realloc(buffer, sizeof(sample_t) * bufferSize);
		}
	}

	int AudioDevice::getBufferSize() {
		return bufferSize;
	}

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
