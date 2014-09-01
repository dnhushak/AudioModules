#include "Filter.hpp"
namespace audio {
	Filter::Filter() {

	}
	
	sample_t * Filter::advance(int numSamples) {
		if (!isEmpty()) {
			copyToBuffer(front()->read(numSamples), numSamples);
			//todo: FILTER CODE!!
		} else {
			zeroBuffer();
		}
		return buffer;
	}

}
