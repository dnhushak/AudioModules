#ifndef LIMITER_h_
#define LIMITER_h_

#include "AudioDevice.h"
#include "Connectable.h"
#include "Alterable.h"

namespace audio {
	using namespace device;
	using namespace std;
	class Clipper: public Alterable<Connectable<AudioDevice, AudioDevice> > {
		public:
			Clipper();

			virtual Clipper * clone();

			void alter(string paramName, Parameter p);

			sample_t * advance();

			void process(const sample_t *inBuffer, sample_t *outBuffer,
					int samplesToProcess, int numChannels);

			float getThreshold();

			void setThreshold(float);

		private:
			sample_t thresholdHi;
			sample_t thresholdLo;
			float threshold;
	};

}

#endif /* LIMITER_h_ */
