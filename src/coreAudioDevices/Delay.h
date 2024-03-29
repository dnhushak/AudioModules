#ifndef DELAY_h_
#define DELAY_h_

#include "AudioDevice.h"
#include "Connectable.h"

namespace audio {
	using namespace device;
	using namespace std;
	class Delay: public Connectable<AudioDevice, AudioDevice> {
		public:
			Delay();

			virtual Delay * clone(int);

			void alter(int paramNum, Parameter p);

			sample_t * advance();

			void setMaxDelayTime(int);

			void setDelayTime(int);

			void setDelaySamples(int);

			int getDelayTime();

			int getDelaySamples();

			virtual ~Delay();
		private:
			void normalizeCurrentPointer();

			void normalizeDelayPointer();

			void zeroDelayBuffer();

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
