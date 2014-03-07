#include "Ramp.hpp"

void chip::Ramp::Ramp(int initBufferSize, int initSampleRate) {
	// Initialize the output buffer
	bufferSize = initBufferSize;

	// Initialize the sample rate
	sampleRate = initSampleRate;

	// Initialize state to INIT
	state = ACTIVE;
}

// Advance the rampelope. Returns a buffer holding the rampelope multiplier values
float * chip::Ramp::advance(int numSamples) {

	for (int i = 0; i < numSamples; i++) {
		if (state == ACTIVE) {
			rampmult += slope;
			// When the evelope location has hit the number of samples, do a state transition
			if (ramploc >= sampCount) {
				state = INACTIVE;
				ramploc = 0;
			}
		}
		buffer[i] = rampmult;
		ramploc++;
	}
	return buffer;
}

// Gets the state of the ramp
chip::devState_t chip::Ramp::getState() {
	return state;
}

// Starts the rampelope
void chip::Ramp::startRamp() {
	state = ACTIVE;
	rampmult = 0.0;
	ramploc = 0;
	slope = (1.0 - rampmult) / sampCount;
}

// Releases the rampelope
void chip::Ramp::stopRamp() {
	state = INACTIVE;
	ramploc = 0;
}

/*** Getters and setters ***/
void chip::Ramp::setTime(int newTime) {
	time = newTime;
	sampCount = (time * sampleRate) / 1000;
}

int chip::Ramp::getTime() {
	return time;
}

