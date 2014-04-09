#include "AudioDevice.hpp"
#include "ConnectableDevice.hpp"
namespace synth {

	class Filter: public AudioDevice, public ConnectableDevice<AudioDevice> {
		public:
			Filter();

			sample_t * advance(int);
	};
}
