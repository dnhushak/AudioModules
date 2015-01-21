#ifndef VCA_h_
#define VCA_h_

#include "AudioDevice.h"
#include "ConnectableDevice.h"

namespace audio {
	
	class VCA: public AudioDevice, public device::ConnectableDevice<AudioDevice, AudioDevice> {
		public:
			VCA();

			VCA * clone();

			sample_t * advance();

			void setGain(float);

		private:
			float gain;
	};
}

#endif //VCA_h_
