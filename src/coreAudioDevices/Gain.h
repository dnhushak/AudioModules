#ifndef GAIN_h_
#define GAIN_h_

#include "AudioDevice.h"
#include "ConnectableDevice.h"

namespace audio {
	
	class Gain: public AudioDevice,
			public device::ConnectableDevice<AudioDevice, Gain> {
		public:
			Gain();

			Gain * clone();

			sample_t * advance();

			void setGain(float);

		private:
			float gain;
	};
}

#endif //GAIN_h_
