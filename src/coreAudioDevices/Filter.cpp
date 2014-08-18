#include "Filter.hpp"
namespace audio {
	Filter::Filter() {

	}
	
	sample_t * Filter::advance(int numSamples) {
		if (!isEmpty()) {
			return front()->advance(numSamples);
			//todo: FILTER CODE!!
		} else {
			zeroBuffer();
			return buffer;
		}
	}

}
