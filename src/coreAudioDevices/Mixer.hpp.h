#ifndef MIXER_HPP_
#define MIXER_HPP_

#include "AudioDevice.hpp"
#include "ConnectableDevice.hpp"

namespace audio {
	class Mixer: public AudioDevice, public device::ConnectableDevice<AudioDevice> {
		public:
			// Constructor
			Mixer();
			// Advance by a given number of samples (in this case summing all in the AudioList
			sample_t * advance();

	};
}

#endif //MIXER_HPP_
