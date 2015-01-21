#ifndef GAIN_h_
#define GAIN_h_

#include "AudioDevice.h"
#include "Connectable.h"

namespace audio {
	
	class Gain: public device::Connectable<AudioDevice, AudioDevice> {
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
