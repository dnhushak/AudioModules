#include "Filter.h"
namespace audio {
	Filter::Filter() {
		setMaxNumDevices(1);
	}
	
	Filter * Filter::clone() {
		// Create new device
		Filter * newDevice = new Filter();
		// Set all member variables
		newDevice->state = this->state;
		// TODO: member variables for filter

		return newDevice;
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
