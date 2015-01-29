#ifndef BITCRUSHER_h_
#define BITCRUSHER_h_
#include "AudioDevice.h"
#include "Connectable.h"
#include "Alterable.h"

namespace audio {
	using namespace device;
	using namespace std;
	
	class BitCrusher: public Alterable<Connectable<AudioDevice, AudioDevice> > {
		public:
			BitCrusher();
			virtual BitCrusher * clone();
			sample_t * advance();
			void alter(string paramName, Parameter p);
			void setBitDepth(int newBitDepth);
			int getBitDepth();
			virtual ~BitCrusher();
		private:
			int bitDepth;
	};

}

#endif /* BITCRUSHER_h_ */
