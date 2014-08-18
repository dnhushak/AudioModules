#ifndef FILTER_HPP_
#define FILTER_HPP_

#include "AudioDevice.hpp"
#include "ConnectableDevice.hpp"
namespace audio {

	class Filter: public AudioDevice, public device::ConnectableDevice<AudioDevice> {
		public:
			Filter();

			sample_t * advance(int);
	};
}
#endif //FILTER_HPP_
