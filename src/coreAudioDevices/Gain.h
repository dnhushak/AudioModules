#ifndef GAIN_h_
#define GAIN_h_

#include "AudioDevice.h"
#include "Connectable.h"

namespace audio {
	using namespace device;
	using namespace std;
	
	class Gain: public Connectable<AudioDevice, AudioDevice> {
		public:
			Gain();

			virtual Gain * clone(int);

			void alter(int paramName, Parameter p);

			sample_t * advance();

			void setGain(float);

		private:
			float gain;
	};
}

#endif //GAIN_h_
