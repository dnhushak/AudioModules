#include "AudioDevice.hpp"

void chip::AudioDevice::resizeBuffer(int newSize) {
	free(buffer);
	bufferSize = newSize;
	buffer = new float[bufferSize];
}

void chip::AudioDevice::changeSampleRate(int newRate) {
	sampleRate = newRate;
}

// Set every value in buffer to 0
void chip::AudioDevice::zeroBuffer() {
	for (int i = 0; i < bufferSize; i++) {
		buffer[i] = 0.0;
	}
}

int chip::AudioDevice::getBufferSize() {
	return bufferSize;
}

int chip::AudioDevice::getSampleRate() {
	return sampleRate;
}
