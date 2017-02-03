#ifndef BITCRUSHER_h_
#define BITCRUSHER_h_
#include "AudioDevice.h"
#include "Connectable.h"

namespace audio {
	using namespace device;
	using namespace std;
	
	class BitCrusher: public Connectable<AudioDevice, AudioDevice>{
		public:
			BitCrusher();
			virtual BitCrusher * clone(int cloneType);
			sample_t * advance();
			void alter(int paramNum, Parameter p);
			void setBitDepth(int newBitDepth);
			int getBitDepth();
			virtual ~BitCrusher();
		private:
			int bitDepth;
	};

}

#endif /* BITCRUSHER_h_ */
