#include "Mixer.hpp"

chip::Mixer::Mixer(int initBufferSize, int initSampleRate) {
	//constructor
	// Initialize the audio buffers
	temp = new float;
	resizeBuffer(initBufferSize);
	changeSampleRate(initSampleRate);
	audioDeviceList = new std::vector<AudioDevice*>(0);


}

float * chip::Mixer::advance(int numSamples) {
	// Zero out the buffer
	zeroBuffer();

	// Vector sum each element of the list of IAudio compatible objects
	for (unsigned int i = 0; i < numAudioDevices; i++) {

		// Fill up a temp buffer for one IAudio object
		temp = audioDeviceList->at(i)->advance(numSamples);

		// Add each element into the mixdown buffer
		for (int j = 0; j < bufferSize; j++) {
			// Sum each advanced IAudio to the master mixed vector
			buffer[j] += temp[j];
		}
	}

	//Pointer to the summed buffer
	return buffer;
}

// Sets the maximum number of devices
void chip::Mixer::setMaxNumAudioDevices(int newMax) {
	// Check if a valid maximum. (-1 indicates no max, 0 indicates no objects)
	if (newMax >= -2) {
		maxNumAudioDevices = newMax;
	}

	// If our new maximum is less than our current number of devices, reduce the current device count
	if (maxNumAudioDevices > -1 && numAudioDevices > maxNumAudioDevices) {
		numAudioDevices = maxNumAudioDevices;
	}
}
