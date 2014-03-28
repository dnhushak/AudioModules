#include "Filter.hpp"
namespace synth {
	Filter::Filter() {

	}
	
	float * Filter::advance(int numSamples) {
		if (getNumAudioDevices() > 0) {
			return audioDeviceList->front()->advance(numSamples);
		} else {
			zeroBuffer();
			return buffer;
		}
	}

}
