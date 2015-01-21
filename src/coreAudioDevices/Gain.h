#ifndef GAIN_h_
#define GAIN_h_

#include "AudioDevice.h"
#include "ConnectableDevice.h"

namespace audio {
	
	class Gain: public device::ConnectableDevice<AudioDevice, AudioDevice> {
		public:
			Gain();

			virtual Gain * clone();

			sample_t * advance();

			void setGain(float);

		private:
			float gain;
	};
}

#endif //GAIN_h_
