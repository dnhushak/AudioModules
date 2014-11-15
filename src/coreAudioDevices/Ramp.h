#ifndef RAMP_h_
#define RAMP_h_

#include "AudioDevice.h"
#include "ConnectableDevice.h"

namespace audio {

	class Ramp: public AudioDevice,
			public device::ConnectableDevice<AudioDevice> {
		public:
			// Constructor
			Ramp();

			// Advance/fill the buffer
			sample_t * advance();

			// Starts the Ramp
			void startRamp();

			// Stops the Ramp
			void stopRamp();

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
#endif //RAMP_h_
