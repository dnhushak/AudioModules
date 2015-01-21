#ifndef FILTER_h_
#define FILTER_h_

#include "AudioDevice.h"
#include "Connectable.h"
namespace audio {

	class Filter: public device::Connectable<AudioDevice, AudioDevice> {
		public:
			Filter();

			virtual Filter * clone();

			sample_t * advance();
	};
}
#endif //FILTER_h_
