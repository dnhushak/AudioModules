#include "BitCrusher.h"

namespace audio {
	
	BitCrusher::BitCrusher() {
		bitDepth = 24;
		setMaxNumDevices(1);
	}
	
	BitCrusher * BitCrusher::clone() {
		// Create new device
		BitCrusher * newDevice = new BitCrusher();
		// Set all member variables
		newDevice->state = this->state;
		newDevice->bitDepth = this->bitDepth;

		return newDevice;
	}

	sample_t * BitCrusher::advance() {
		if (!isEmpty()) {
			copyToBuffer(front()->read(), bufferSize);
			// Multiply by INT max, divide by sample max, cast to
			//TODO: Fix bitcrush
		} else {
			zeroBuffer();
		}
		return buffer;
	}

	void BitCrusher::alter(int paramNum, Parameter p) {
		switch (paramNum) {
			case 0:
				// Bit Depth
				setBitDepth(p.getParam().i);
				break;
		}
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
