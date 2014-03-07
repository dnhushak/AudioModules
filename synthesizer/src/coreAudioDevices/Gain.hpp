#include "AudioEffect.hpp"
#include "AudioUtils.h"

namespace chip {
	
	class Gain: public chip::AudioEffect {
		public:
			Gain(int, int);

			float * advance(int);

			void setGain(float);

		private:
			float gain;
	};

}
