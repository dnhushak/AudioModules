#include "Mixer.hpp"

void chip::Mixer::Mixer(int initBufferSize, int initSampleRate) {
	//constructor
	audioDeviceList = new std::vector<AudioDevice*>(0);

	bufferSize = initBufferSize;
	sampleRate = initSampleRate;

	// Initialize the audio buffer
	buffer = new float[bufferSize];
	temp = new float;
}

float * chip::Mixer::advance(int numSamples) {

	// Zero out the buffer
	zeroBuffer();

	// Vector sum each element of the list of IAudio compatible objects
	for (unsigned int i = 0; i < audioDeviceList->size(); i++) {
		// Fill up a temp buffer for one IAudio object
		temp = (*audioDeviceList)[i]->advance(numSamples);

		// Add each element into the mixdown buffer
		for (int j = 0; j < bufferSize; j++) {
			// Sum each advanced IAudio to the master mixed vector
			buffer[j] += temp[j];
		}
	}

	//Pointer to the summed buffer
	return buffer;
}
