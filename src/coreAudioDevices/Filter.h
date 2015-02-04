#ifndef FILTER_h_
#define FILTER_h_

#include "AudioDevice.h"
#include "Connectable.h"
#include "Alterable.h"
namespace audio {
	using namespace device;
	using namespace std;

	class Filter: public Connectable<AudioDevice, AudioDevice>{
		public:
			Filter();

			virtual Filter * clone();

			sample_t * advance();
	};
}
#endif //FILTER_h_
