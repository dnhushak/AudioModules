#include "AudioEngine.hpp"

namespace audio {
	
	AudioEngine::AudioEngine() {
		// TODO Auto-generated constructor stub
		
	}
	
	sample_t * AudioEngine::advance(){
		return buffer;
	}

	AudioEngine::~AudioEngine() {
		// TODO Auto-generated destructor stub
	}

}
