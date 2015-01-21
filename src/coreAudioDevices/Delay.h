#ifndef DELAY_h_
#define DELAY_h_

#include "AudioDevice.h"
#include "ConnectableDevice.h"

namespace audio {
	
	class Delay: public device::ConnectableDevice<AudioDevice, AudioDevice> {
		public:
			Delay();

			virtual Delay * clone();

			sample_t * advance();

			void setMaxDelayTime(int);

			void setDelayTime(int);

			void setDelaySamples(int);

			void normalizeCurrentPointer();

			void normalizeDelayPointer();

			void zeroDelayBuffer();

			int getDelayTime();

			int getDelaySamples();

			virtual ~Delay();
		private:
			sample_t * delayBuffer;

			int delayBufferSize;

			// The pointer offset to write to the delay buffer (input)
			int curSample;

			// The pointer offset to read from the delay buffer (output)
			int curDelaySample;

			int delayTime;

			int delaySamples;
	};

}

#endif /* DELAY_h_ */
