#ifndef GAIN_HPP_
#define GAIN_HPP_

#include "AudioDevice.hpp"
#include "ConnectableDevice.hpp"

namespace audio {
	
	class Gain: public AudioDevice, public device::ConnectableDevice<AudioDevice> {
		public:
			Gain();

			sample_t * advance(int);

			void setGain(float);

		private:
			float gain;
	};
}

#endif //GAIN_HPP_
