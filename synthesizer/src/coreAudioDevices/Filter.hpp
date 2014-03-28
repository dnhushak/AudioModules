#include "AudioEffect.hpp"
namespace synth {

	class Filter: public AudioEffect {
		public:
			Filter();

			float * advance(int);
	};
}
