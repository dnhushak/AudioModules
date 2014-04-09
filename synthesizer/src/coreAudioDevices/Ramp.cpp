#include "Ramp.hpp"
namespace synth {
	Ramp::Ramp() {
		// Restrict to only one audio device;
		setMaxNumDevices(1);

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

	// Advance the ramp. Returns a buffer of the new ramp-scaled values
	sample_t * Ramp::advance(int numSamples) {
		if (!isEmpty()) {
			buffer = front()->advance(numSamples);
			for (int i = 0; i < numSamples; i++) {
				if (state == ACTIVE) {
					rampmult += slope;
					// When the evelope location has hit the number of samples, do a state transition
					if (ramploc >= sampCount) {
						state = INACTIVE;
						ramploc = 0;
					}
				}
				buffer[i] *= (sample_t) rampmult;
				ramploc++;
			}
		} else {
			// If no devices connected
			zeroBuffer();
		}
		return buffer;
	}

	// Starts the ramp
	void Ramp::startRamp() {
		state = ACTIVE;
		rampmult = 0.0;
		ramploc = 0;
		slope = (1.0 - rampmult) / sampCount;
	}

	// Releases the ramp
	void Ramp::stopRamp() {
		state = INACTIVE;
		ramploc = 0;
	}

	/*** Getters and setters ***/
	void Ramp::setTime(int newTime) {
		if (newTime > 0) {
			time = newTime;
			sampCount = (time * sampleRate) / 1000;
		}
	}

	int Ramp::getTime() {
		return time;
	}
}

