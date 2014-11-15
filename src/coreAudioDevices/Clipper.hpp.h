#ifndef LIMITER_HPP_
#define LIMITER_HPP_

#include "AudioDevice.hpp"
#include "ConnectableDevice.hpp"

namespace audio {
	
	class Clipper: public AudioDevice, public device::ConnectableDevice<AudioDevice> {
		public:
			Clipper();
			sample_t * advance();
			void setThreshold(float);
		private:
			sample_t thresholdHi;
			sample_t thresholdLo;
	};

}

#endif /* LIMITER_HPP_ */
