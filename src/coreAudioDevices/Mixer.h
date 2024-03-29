#ifndef MIXER_h_
#define MIXER_h_

#include "AudioDevice.h"
#include "Connectable.h"

namespace audio {
	using namespace device;
	class Mixer: public Connectable<AudioDevice, AudioDevice> {
		public:
			// Constructor
			Mixer();

			virtual Mixer * clone(int);

			// Advance by a given number of samples (in this case summing all in the AudioList
			sample_t * advance();

//			virtual void process(const sample_t * *inBuffers, int numInBuffers,
//					sample_t *outBuffer, int samplesToProcess,
//					int numInChannels);

	};
}

#endif //MIXER_h_
