#pragma once
#include "AudioDevice.hpp"
#include <vector>
#include <iostream>

namespace chip {
	class Ramp: public AudioDevice {
		public:
			// Constructor
			Ramp(int, int);

			// Advance/fill the buffer
			float * advance(int);

			// Gets the current state
			devState_t getState();

			// Starts the Ramp
			void startRamp();

			// Stops the Ramp
			void stopRamp();

			// Sets/gets the envelope properties
			void setTime(int);
			int getTime();

		private:

			// Current state of ramp
			devState_t state;

			// time of ramp
			int time;

			// Envelope location
			int ramploc;

			// The current envelope multiplier (goes from 0 to 1)
			float rampmult;

			// Length (in samples) of the ramp
			int sampCount;

			// Slope of the ramp curve
			float slope;
	};
}
