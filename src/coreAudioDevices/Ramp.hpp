#pragma once
#include "AudioDevice.hpp"
#include "ConnectableDevice.hpp"
#include <vector>
#include <iostream>

namespace synth {
	class Ramp: public AudioDevice, public ConnectableDevice<AudioDevice> {
		public:
			// Constructor
			Ramp();

			// Advance/fill the buffer
			sample_t * advance(int);

			// Starts the Ramp
			void startRamp();

			// Stops the Ramp
			void stopRamp();

			// Sets/gets the ramp properties
			void setTime(int);
			int getTime();

		private:
			// time of Ramp in ms
			int time;

			// Ramp location
			int ramploc;

			// The current ramp multiplier (goes from 0 to 1)
			float rampmult;

			// Length (in samples) of the ramp
			int sampCount;

			// Slope of the ramp curve
			float slope;
	};
}
