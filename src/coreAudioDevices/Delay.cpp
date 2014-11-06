#include "Delay.hpp"

namespace audio {
	Delay::Delay() {
		setMaxNumDevices(1);

		// Initialize the delay buffer;
		delayBuffer = (sample_t *) malloc(sizeof(sample_t) * bufferSize);
		// Default maximum delay is 2 seconds
		setMaxDelayTime(2000);
		zeroDelayBuffer();
		// Start the input writing at the beginning of the delay buffer;
		curSample = 0;
		// Default to 0 delay;
		setDelaySamples(0);
	}
	
	sample_t* Delay::advance() {
		for (int i = 0; i < bufferSize; i++) {
			// Fill the delay buffer with appropriate samples from input
			if (!isEmpty()) {
				// Attached device, sample by sample
				delayBuffer[curSample++] = front()->read()[i];
			} else {
				// No attached device, buffer is filled with zeros
				delayBuffer[curSample++] = 0;
			}
			// After incrementing the curSample pointer,
			// normalize it so we don't access outside of the delay buffer array;
			normalizeCurrentPointer();

			// Copy the delay buffer over to the regular buffer
			buffer[i] = delayBuffer[curDelaySample++];
			// Normalize delay pointer for array access reasons
			normalizeDelayPointer();
		}
		return buffer;
	}
	
	void Delay::setMaxDelayTime(int newMaxTime) {
		if (newMaxTime > 0) {
			// Convert to samples
			delayBufferSize = msToSamp(newMaxTime, sampleRate);
			// Resize the buffer - since using realloc, maintains whatever
			// current buffer is, minus whatever gets truncated if buffer is shrinking
			delayBuffer = (sample_t *) realloc(delayBuffer,
					sizeof(sample_t) * delayBufferSize);
		}
	}

	void Delay::normalizeCurrentPointer() {
		curSample %= delayBufferSize;
	}

	void Delay::normalizeDelayPointer() {
		curDelaySample %= delayBufferSize;
	}

	void Delay::setDelayTime(int newTime) {
		if (newTime > 0) {
			// Set the new time and sample values
			delayTime = newTime;
			delaySamples = msToSamp(newTime, sampleRate);

			// Adjust the delay pointer to delay correctly
			curDelaySample = curSample - delaySamples;
			normalizeDelayPointer();
		}
	}
	
	void Delay::setDelaySamples(int newSamples) {
		if (newSamples > 0) {
			// Set the new time and sample values
			delayTime = sampToMs(newSamples, sampleRate);
			delaySamples = newSamples;

			// Adjust the delay pointer to delay correctly
			curDelaySample = curSample - delaySamples;
			normalizeDelayPointer();
		}
	}
	
	int Delay::getDelayTime() {
		return delayTime;
	}
	
	int Delay::getDelaySamples() {
		return delaySamples;
	}
	
	void Delay::zeroDelayBuffer() {
		memset(delayBuffer, 0, sizeof(sample_t) * delayBufferSize);
	}

	Delay::~Delay() {
		free(delayBuffer);
	}

}
