#include "AudioEffect.hpp"

class Filter: public chip::AudioEffect {
	public:
		Filter();

		float * advance(int);
};
