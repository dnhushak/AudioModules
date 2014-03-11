#include "AudioEffect.hpp"
#include "AudioUtils.hpp"

namespace chip {
	
	class Gain: public AudioEffect {
		public:
			Gain(int, int);

			float * advance(int);

			void setGain(float);

		private:
			float gain;
	};

}
