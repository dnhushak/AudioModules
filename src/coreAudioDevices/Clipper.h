#ifndef LIMITER_h_
#define LIMITER_h_

#include "AudioDevice.h"
#include "Connectable.h"

namespace audio {
	using namespace device;
	using namespace std;
	class Clipper: public Connectable<AudioDevice, AudioDevice> {
		public:
			Clipper();

			virtual Clipper * clone(int);

			void alter(int paramName, Parameter p);

			sample_t * advance();

			float getThreshold();

			void setThreshold(float);

		private:
			sample_t thresholdHi;
			sample_t thresholdLo;
			float threshold;
	};

}

#endif /* LIMITER_h_ */
