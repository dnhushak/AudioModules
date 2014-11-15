#ifndef LIMITER_h_
#define LIMITER_h_

#include "AudioDevice.h"
#include "ConnectableDevice.h"

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

#endif /* LIMITER_h_ */
