#ifndef BITCRUSHER_HPP_
#define BITCRUSHER_HPP_
#include "AudioDevice.hpp"
#include "ConnectableDevice.hpp"

namespace audio {
	
	class BitCrusher: public audio::AudioDevice,
			public device::ConnectableDevice<AudioDevice> {
		public:
			BitCrusher();
			sample_t * advance();
			void setBitDepth(int newBitDepth);
			int getBitDepth();
			virtual ~BitCrusher();
		private:
			int bitDepth;
	};

}

#endif /* BITCRUSHER_HPP_ */
