#include "../src/AudioDevice.hpp"
#include <vector>

namespace synth {
	class MockIAudio: public AudioDevice {
		public:

			MockIAudio(int);
			float * advance(int num);

			float * buffer;

	};
}
