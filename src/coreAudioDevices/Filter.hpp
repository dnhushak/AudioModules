#include "AudioDevice.hpp"
#include "ConnectableDevice.hpp"
namespace modules {

	class Filter: public AudioDevice, public ConnectableDevice<AudioDevice> {
		public:
			Filter();

			sample_t * advance(int);
	};
}
