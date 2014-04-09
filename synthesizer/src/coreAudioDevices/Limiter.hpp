#ifndef LIMITER_HPP_
#define LIMITER_HPP_
#include "AudioDevice.hpp"
#include "ConnectableDevice.hpp"

namespace synth {
	
	class Limiter: public AudioDevice, public ConnectableDevice<AudioDevice> {
		public:
			Limiter();
			sample_t * advance(int);
			void setThreshold(sample_t);
		private:
			sample_t thresholdHi;
			sample_t thresholdLo;
	};

}

#endif /* LIMITER_HPP_ */
