#ifndef BITCRUSHER_h_
#define BITCRUSHER_h_
#include "AudioDevice.h"
#include "ConnectableDevice.h"

namespace audio {
	
	class BitCrusher: public device::ConnectableDevice<AudioDevice, AudioDevice> {
		public:
			BitCrusher();
			virtual BitCrusher * clone();
			sample_t * advance();
			void setBitDepth(int newBitDepth);
			int getBitDepth();
			virtual ~BitCrusher();
		private:
			int bitDepth;
	};

}

#endif /* BITCRUSHER_h_ */
