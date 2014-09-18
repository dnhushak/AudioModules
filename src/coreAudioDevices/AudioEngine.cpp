#include "AudioEngine.hpp"

namespace audio {
	
	AudioEngine::AudioEngine() {
		// TODO Auto-generated constructor stub
		
	}
	
	void AudioEngine::endOfBuffer() {

		deviceIter = begin();
		deviceIter++;
		// Start at the second item
		while (deviceIter != end()) {
			(*deviceIter)->resetAdvanceBit();
		}
	}

	AudioEngine::~AudioEngine() {
		// TODO Auto-generated destructor stub
	}

}
