#include "Filter.hpp"

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

