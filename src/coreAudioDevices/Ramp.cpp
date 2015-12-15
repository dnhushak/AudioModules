#include "Ramp.h"
namespace audio {
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

		stopRamp();
	}

	Ramp * Ramp::clone(int cloneType) {
		if (cloneType != 0) {
			return (Ramp *) Connectable::clone(cloneType);
		}
		// Create new device
		Ramp * newDevice = new Ramp();
		// Set all member variables
		newDevice->state = this->state;
		newDevice->setTime(this->time);
		newDevice->ramploc = this->ramploc;
		return newDevice;

	}

	void Ramp::alter(int paramNum, Parameter p) {
		switch (paramNum) {
			case 0:
				//Time
				setTime(p.getParam().i);
				break;
			case 1:
				// Start Ramp
				startRamp();
				break;
			case 2:
				// Stop Ramp
				stopRamp();
				break;
		}
	}

	// Advance the ramp. Returns a buffer of the new ramp-scaled values
	sample_t * Ramp::advance() {
		if (!isEmpty()) {
			copyToBuffer(front()->read(), bufferSize);

			for (int i = 0; i < bufferSize; i++) {
				if (state == device::ACTIVE) {
					rampmult += slope;
					// When the evelope location has hit the number of samples, do a state transition
					if (ramploc >= sampCount) {
						stopRamp();
					}
				}
				buffer[i] *= rampmult;
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
		state = device::ACTIVE;
		rampmult = 0.0;
		ramploc = 0;
		slope = (1.0 - rampmult) / sampCount;
	}

	// Releases the ramp
	void Ramp::stopRamp() {
		state = device::INACTIVE;
		ramploc = 0;
	}

	void Ramp::setTime(int newTime) {
		if (newTime > 0) {
			time = newTime;
			sampCount = msToSamp(time, sampleRate);
			// Adjust the slope to the new time, if the ramp still hasn't completed
			slope = (1.0 - rampmult) / (sampCount - ramploc);
		}
	}

	int Ramp::getTime() {
		return time;
	}
}

