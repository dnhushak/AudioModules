#include "Filter.hpp"
namespace audio {
	Filter::Filter() {

	}
	
	sample_t * Filter::advance() {
		if (!isEmpty()) {
			copyToBuffer(front()->read(), bufferSize);
			//TODO: FILTER CODE!!
		} else {
			zeroBuffer();
		}
		return buffer;
	}

}
