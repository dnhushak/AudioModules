#ifndef VCA_h_
#define VCA_h_

#include "AudioDevice.h"
#include "Connectable.h"
#include "Alterable.h"

namespace audio {
	using namespace device;
	using namespace std;
	
	class VCA: public Alterable<Connectable<AudioDevice, AudioDevice> >{
		public:
			VCA();

			virtual VCA * clone();

			void alter(int paramNum, Parameter p);

			sample_t * advance();

			void setGain(float);

		private:
			float gain;
	};
}

#endif //VCA_h_
