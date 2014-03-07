#include "MIDIDevice.hpp"

float * chip::MIDIDevice::advance(int numSamples) {
	for (int i = 0; i < numSamples; i++) {
		buffer[i] = 0;
	}
	return buffer;
}

// Resize the buffer of the audio device
// DO NOT CALL DURING CALLBACK
void chip::MIDIDevice::resizeBuffer(int newSize) {
	// Clears old buffer memory
	free(buffer);
	bufferSize = newSize;
	buffer = new float[bufferSize];
}

// Change the sample rate of the device
void chip::MIDIDevice::changeSampleRate(int newRate) {
	sampleRate = newRate;
}

// Set every value in buffer to 0
void chip::MIDIDevice::zeroBuffer() {
	for (int i = 0; i < bufferSize; i++) {
		buffer[i] = 0.0;
	}
}

// Return the buffer size
int chip::MIDIDevice::getBufferSize() {
	return bufferSize;
}

// Return the audio sampling rate
int chip::MIDIDevice::getSampleRate() {
	return sampleRate;
}
