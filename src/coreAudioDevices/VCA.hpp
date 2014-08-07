#ifndef VCA_HPP_
#define VCA_HPP_

#include "AudioDevice.hpp"
#include "ConnectableDevice.hpp"

namespace modules {
	
	class VCA: public AudioDevice, public ConnectableDevice<AudioDevice> {
		public:
			VCA();

			sample_t * advance(int);
	};
}

#endif //VCA_HPP_
