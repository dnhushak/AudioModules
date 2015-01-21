#ifndef FILTER_h_
#define FILTER_h_

#include "AudioDevice.h"
#include "ConnectableDevice.h"
namespace audio {

	class Filter: public AudioDevice, public device::ConnectableDevice<AudioDevice, Filter> {
		public:
			Filter();

			Filter * clone();

			sample_t * advance();
	};
}
#endif //FILTER_h_
