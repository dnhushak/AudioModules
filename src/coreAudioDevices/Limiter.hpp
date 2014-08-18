#ifndef LIMITER_HPP_
#define LIMITER_HPP_

#include "AudioDevice.hpp"
#include "ConnectableDevice.hpp"

namespace audio {
	
	class Limiter: public AudioDevice, public device::ConnectableDevice<AudioDevice> {
		public:
			Limiter();
			sample_t * advance(int);
			void setThreshold(float);
		private:
			sample_t thresholdHi;
			sample_t thresholdLo;
	};

}

#endif /* LIMITER_HPP_ */
