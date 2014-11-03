#include "AudioEngine.hpp"

namespace audio {
	
	AudioEngine::AudioEngine() {
		
	}
	
	void AudioEngine::endOfBuffer() {
		//TODO: Actually make this work. Every audio device needs to be added to the AudioEngine list
		deviceIter = begin();
		deviceIter++;
		// Start at the second item
		while (deviceIter != end()) {
			(*deviceIter)->cleanup();
		}
	}

	AudioEngine::~AudioEngine() {
	}

}
