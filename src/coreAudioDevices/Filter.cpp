#include "Filter.hpp"
namespace synth {
	Filter::Filter() {

	}
	
	sample_t * Filter::advance(int numSamples) {
		if (!isEmpty()) {
			return front()->advance(numSamples);
		} else {
			zeroBuffer();
			return buffer;
		}
	}

}
