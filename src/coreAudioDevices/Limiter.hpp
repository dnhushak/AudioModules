#ifndef LIMITER_HPP_
#define LIMITER_HPP_
#include "AudioDevice.hpp"
#include "ConnectableDevice.hpp"

namespace modules {
	
	class Limiter: public AudioDevice, public ConnectableDevice<AudioDevice> {
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
