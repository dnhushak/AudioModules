#include <iostream>
#include <vector>
#include "AudioDevice.hpp"
#include "ConnectableDevice.hpp"

namespace modules {
	
	class Gain: public AudioDevice, public ConnectableDevice<AudioDevice> {
		public:
			Gain();

			sample_t * advance(int);

			void setGain(float);

		private:
			float gain;
	};

}
