#ifndef FILTER_h_
#define FILTER_h_

#include "AudioDevice.h"
#include "ConnectableDevice.h"
namespace audio {

	class Filter: public device::ConnectableDevice<AudioDevice, AudioDevice> {
		public:
			Filter();

			virtual Filter * clone();

			sample_t * advance();
	};
}
#endif //FILTER_h_
