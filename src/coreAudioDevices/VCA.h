#ifndef VCA_h_
#define VCA_h_

#include "AudioDevice.h"
#include "Connectable.h"

namespace audio {
	using namespace device;
	using namespace std;
	
	class VCA: public Connectable<AudioDevice, AudioDevice>{
		public:
			VCA();

			virtual VCA * clone(int);

			void alter(int paramNum, Parameter p);

			sample_t * advance();

			void setGain(float);

		private:
			float gain;
	};
}

#endif //VCA_h_
