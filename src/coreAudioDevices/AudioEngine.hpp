#ifndef AUDIOENGINE_HPP_
#define AUDIOENGINE_HPP_

#include "AudioDevice.hpp"
#include "ConnectableDevice.hpp"

namespace audio {
	
	class AudioEngine: public audio::AudioDevice,
			public device::ConnectableDevice<AudioDevice> {
		public:
			AudioEngine();
			sample_t * advance();
			virtual ~AudioEngine();
	};

}

#endif /* AUDIOENGINE_HPP_ */
