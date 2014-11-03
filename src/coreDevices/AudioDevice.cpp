#include "AudioDevice.hpp"

namespace audio {
	AudioDevice::AudioDevice() {
		// Add the device to the audio engine, so that at the end of every buffer update,
		// all audio devices can be remarked as "not advanced"

		// Allocate space for the device's output buffer
		buffer = (sample_t *) malloc(sizeof(sample_t) * bufferSize);
		zeroBuffer();
	}

	void AudioDevice::cleanup() {
		advanced = 0;
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
			bufferSize = newSize;

			/* Use realloc so any samples currently in the buffer stay in it,
			 * and the buffer is extended. The buffer is truncated if the new size
			 * is smaller
			 */
			buffer = (sample_t *) realloc(buffer,
					sizeof(sample_t) * bufferSize);
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
		memcpy(buffer, otherBuffer, sizeof(sample_t) * numSamples);
	}

	AudioDevice::~AudioDevice() {
		free(buffer);
	}
}
