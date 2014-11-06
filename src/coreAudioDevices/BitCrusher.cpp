#include "BitCrusher.hpp"

namespace audio {
	
	BitCrusher::BitCrusher() {
		bitDepth = 24;
	}
	
	sample_t * BitCrusher::advance() {
		if (!isEmpty) {
			copyToBuffer(front()->read(), bufferSize);
			// Multiply by INT max, divide by sample max, cast to
		} else {
			zeroBuffer();
		}
		return buffer;
	}

	void BitCrusher::setBitDepth(int newBitDepth) {
		if (newBitDepth > 0 & newBitDepth < 25) {
			bitDepth = newBitDepth;
		}
	}
	int BitCrusher::getBitDepth() {
		return bitDepth;
	}

	BitCrusher::~BitCrusher() {
	}

}
