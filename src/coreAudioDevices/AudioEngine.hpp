#ifndef AUDIOENGINE_HPP_
#define AUDIOENGINE_HPP_

#include "AudioDevice.hpp"

namespace audio {
	
	class AudioEngine: public audio::AudioDevice,
			public device::ConnectableDevice<AudioDevice> {
		public:
			AudioEngine();
			virtual ~AudioEngine();
	};

}

#endif /* AUDIOENGINE_HPP_ */
