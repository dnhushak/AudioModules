#include "AudioDevice.h"

namespace audio {

	AudioDevice::AudioDevice() : bufferSize (64), sampleRate (44100){
		// Allocate space for the device's output buffer
		buffer = (sample_t *) malloc(sizeof(sample_t) * bufferSize);
		audioDeviceList.push_back(this);
		zeroBuffer();
		advanced = 0;
	}

	void AudioDevice::cleanup() {
	}

	sample_t * AudioDevice::read() {
		// Check if buffer is updated - if it isn't, update it
		if (!advanced) {
			advance();
			advanced = 1;
		}
		return buffer;
	}

	void AudioDevice::zeroBuffer() {
		memset(buffer, 0, sizeof(sample_t) * bufferSize);
	}

	void AudioDevice::setBuffer(sample_t sampVal) {
		memset(buffer, sampVal, sizeof(sample_t) * bufferSize);
	}

	void AudioDevice::setBufferSize(int newSize) {
		if (newSize > 0) {
			for (AudioDevice * audioDevice : audioDeviceList) {
				audioDevice->bufferSize = newSize;
				audioDevice->resizeBuffer();
			}
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

	int AudioDevice::getSampleRate() {
		return sampleRate;
	}

	void AudioDevice::copyToBuffer(sample_t * otherBuffer, int numSamples) {
		if (numSamples > bufferSize) {
			numSamples = bufferSize;
		}
		memcpy(buffer, otherBuffer, sizeof(sample_t) * numSamples);
	}

	void AudioDevice::resizeBuffer() {
		/* Use realloc so any samples currently in the buffer stay in it,
		 * and the buffer is extended. The buffer is truncated if the new size
		 * is smaller
		 */
		buffer = (sample_t *) realloc(buffer, sizeof(sample_t) * bufferSize);
	}

	void AudioDevice::endOfBuffer() {
		for (AudioDevice * audioDevice : audioDeviceList) {
			audioDevice->resetAdvanceBit();
			audioDevice->cleanup();
		}
	}

	void AudioDevice::resetAdvanceBit() {
		advanced = 0;
	}

	AudioDevice::~AudioDevice() {
		audioDeviceList.remove(this);
		free(buffer);
	}
}
