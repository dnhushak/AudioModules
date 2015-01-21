#ifndef LIMITER_h_
#define LIMITER_h_

#include "AudioDevice.h"
#include "Connectable.h"

namespace audio {
	
	class Clipper: public device::Connectable<AudioDevice, AudioDevice> {
		public:
			Clipper();
			virtual Clipper * clone();
			sample_t * advance();
			void setThreshold(float);
		private:
			sample_t thresholdHi;
			sample_t thresholdLo;
	};

}

#endif /* LIMITER_h_ */
