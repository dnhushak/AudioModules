#ifndef INTERLEAVER_H_
#define INTERLEAVER_H_

#include "src/coreAudioDevices/AudioProcessor.h"

namespace audio {
	
	class Interleaver: public audio::AudioProcessor {
		public:
			Interleaver();
			virtual void process(
					const sample_t * * inputBuffers,
					int numInBuffers,
					int numInChannels,
					sample_t * * outputBuffers,
					int numOutBuffers,
					int numOutChannels,
					int numSamples);
			virtual ~Interleaver();
	};

}

#endif /* INTERLEAVER_H_ */
