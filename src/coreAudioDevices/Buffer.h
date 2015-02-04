#ifndef BUFFER_H_
#define BUFFER_H_
#include "AudioDevice.h"

namespace audio {
	
	class Buffer: public AudioDevice {
		public:
			Buffer();
			virtual sample_t * advance();
			Buffer * clone();
			void setSample(sample_t sample, int bufferLoc);
			sample_t getSample(int bufferLoc);
			virtual ~Buffer();
	};

}

#endif /* BUFFER_H_ */
