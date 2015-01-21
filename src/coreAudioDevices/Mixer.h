#ifndef MIXER_h_
#define MIXER_h_

#include "AudioDevice.h"
#include "ConnectableDevice.h"

namespace audio {
	class Mixer: public device::ConnectableDevice<AudioDevice, AudioDevice> {
		public:
			// Constructor
			Mixer();

			virtual Mixer * clone();

			// Advance by a given number of samples (in this case summing all in the AudioList
			sample_t * advance();

	};
}

#endif //MIXER_h_
