#ifndef AUDIOENGINE_HPP_
#define AUDIOENGINE_HPP_

#include "AudioDevice.hpp"
#include "ConnectableDevice.hpp"
namespace audio {
	
	class AudioEngine: public device::ConnectableDevice<AudioDevice> {
		public:
			AudioEngine();
			void endOfBuffer();
			virtual ~AudioEngine();
	};

}

#endif /* AUDIOENGINE_HPP_ */
