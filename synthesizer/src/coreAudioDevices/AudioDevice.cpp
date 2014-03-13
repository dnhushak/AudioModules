#include "AudioDevice.hpp"

chip::AudioDevice::AudioDevice() {
	sampleRate = 0;
	state = ACTIVE;
	bufferSize = 256;
	buffer = (float *) malloc(sizeof(float *));
	resizeBuffer(bufferSize);
}

// Default behavior for audio device, just returns 0
float * chip::AudioDevice::advance(int numSamples) {
	for (int i = 0; i < numSamples; i++) {
		buffer[i] = 0;
	}
	return buffer;
}

chip::devState_t chip::AudioDevice::getState() {
	return state;
}

// Perform cleanup on all devices in audio device list
void chip::AudioDevice::cleanup() {
	return;
}

// Resize the buffer of the audio device
// DO NOT CALL DURING CALLBACK
void chip::AudioDevice::resizeBuffer(int newSize) {
	// Clears old buffer memory
	free(buffer);
	bufferSize = newSize;
	buffer = (float*) malloc(sizeof(float[bufferSize]));
}

// Change the sample rate of the device
void chip::AudioDevice::changeSampleRate(int newRate) {
	sampleRate = newRate;
}

// Set every value in buffer to 0
void chip::AudioDevice::zeroBuffer() {
	for (int i = 0; i < bufferSize; i++) {
		buffer[i] = 0.0;
	}
}

// Return the buffer size
int chip::AudioDevice::getBufferSize() {
	return bufferSize;
}

// Return the audio sampling rate
int chip::AudioDevice::getSampleRate() {
	return sampleRate;
}

void cleanup() {
	return;
}

chip::AudioDevice::~AudioDevice() {
	free(buffer);
}
