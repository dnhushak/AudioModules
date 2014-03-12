#include "Ramp.hpp"

chip::Ramp::Ramp(int initBufferSize, int initSampleRate) {
	changeSampleRate(initSampleRate);
	resizeBuffer(initBufferSize);

	// Initialize state to INIT
	state = ACTIVE;

	// time of ramp
	time = 0;

	// Envelope location
	ramploc = 0;

	// The current envelope multiplier (goes from 0 to 1)
	rampmult = 0;

	// Length (in samples) of the ramp
	sampCount = 100;

	// Slope of the ramp curve
	slope = .01;
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

