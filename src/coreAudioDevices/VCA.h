#ifndef VCA_h_
#define VCA_h_

#include "AudioDevice.h"
#include "Connectable.h"

namespace audio {
	
	class VCA: public device::Connectable<AudioDevice, AudioDevice> {
		public:
			VCA();

			virtual VCA * clone();

			sample_t * advance();

			void setGain(float);

		private:
			float gain;
	};
}

#endif //VCA_h_
