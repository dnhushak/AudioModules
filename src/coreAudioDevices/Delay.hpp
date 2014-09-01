#ifndef DELAY_HPP_
#define DELAY_HPP_

#include "AudioDevice.hpp"
#include "ConnectableDevice.hpp"

namespace audio {
	
	class Delay: public audio::AudioDevice, public device::ConnectableDevice<
			AudioDevice> {
		public:
			Delay();

			sample_t * advance(int);

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

#endif /* DELAY_HPP_ */
