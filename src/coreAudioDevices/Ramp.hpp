#ifndef RAMP_HPP_
#define RAMP_HPP_

#include "AudioDevice.hpp"
#include "ConnectableDevice.hpp"

namespace audio {
	class Ramp: public AudioDevice,
			public device::ConnectableDevice<AudioDevice> {
		public:
			// Constructor
			Ramp();

			// Advance/fill the buffer
			sample_t * advance(int);

			// Starts the Ramp
			void startRamp(int);

			// Stops the Ramp
			void stopRamp(int);

			/**
			 * Sets the time that the ramp takes from start to finish
			 * @param newTime Time in milliseconds of ramp. Must be > 0
			 */
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
#endif //RAMP_HPP_
