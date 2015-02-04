#include "Interleaver.h"

namespace audio {
	
	Interleaver::Interleaver() {
		// TODO Auto-generated constructor stub
		
	}
	
	void Interleaver::process(
			const sample_t** inputBuffers,
			int numInBuffers,
			int numInChannels,
			sample_t** outputBuffers,
			int numOutBuffers,
			int numOutChannels,
			int numSamples) {

		int frameSize = numInBuffers * numInChannels;
		int sampleCounter = frameSize * numSamples;

		// For every sample frame...
		for (int i = 0; i < numSamples; i++) {
			// Go through each input buffer...
			for (int j = numInBuffers - 1; j >= 0; j--) {
				// ...And each buffers' already interleaved channels
				for (int k = numInChannels - 1; k >= 0; k--) {
					// ... And copy them over to the newly interleaved output buffer

					//TODO: Fix this - doesn't grab from the end of the inputBuffers like it should
					outputBuffers[0][sampleCounter] = inputBuffers[j][i*k];
					sampleCounter--;
				}
			}
		}

	}

	Interleaver::~Interleaver() {
		// TODO Auto-generated destructor stub
	}

}
