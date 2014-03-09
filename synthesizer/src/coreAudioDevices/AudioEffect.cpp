#include "AudioEffect.hpp"

// Perform cleanup on all devices in audio device list
void chip::AudioEffect::cleanup(){
	for(int i=0;i<numAudioDevices; i++){
		(*audioDeviceList)[i]->cleanup();
	}
}

// Add another AudioDevice object to be mixed
void chip::AudioEffect::addAudioDevice(AudioDevice* audioObject) {
	// Ignore if at maximum value or no maximum
	if (numAudioDevices < maxNumAudioDevices || maxNumAudioDevices == -1) {
		audioDeviceList->push_back(audioObject);
		numAudioDevices = audioDeviceList->size();
	}
}

// Add a vector of AudioDevices to be mixed
void chip::AudioEffect::addAudioDevices(
		std::vector<AudioDevice*> * audioObjects) {
	int numToAdd;

	// If no maximum...
	if (maxNumAudioDevices != -1) {
		// Add all objects
		numToAdd = audioObjects->size();
	}
	// ...Or number of devices won't put us at our maximum
	else if ((numAudioDevices + audioObjects->size()) < maxNumAudioDevices) {
		// Add all objects
		numToAdd = audioObjects->size();
	}
	// Else add as many as are available
	else {
		numToAdd = maxNumAudioDevices - numAudioDevices;
	}

	audioDeviceList->insert(audioDeviceList->end(), audioObjects->begin(),
			audioObjects->begin() + numToAdd);

	numAudioDevices = audioDeviceList->size();
}

// Switch pointer to a new audioDeviceList
void chip::AudioEffect::setAudioDeviceList(
		std::vector<AudioDevice*> * audioObjects) {

// Re-reference
	audioDeviceList = audioObjects;

// If the new device list has less than the maximum number of devices, or there is no maximum...
	if (audioDeviceList->size() < maxNumAudioDevices
			|| maxNumAudioDevices == -1) {
		// Then the current number of devices is the size of the new list
		numAudioDevices = audioDeviceList->size();
	} else {
		// Otherwise set to maximum number of devices
		numAudioDevices = maxNumAudioDevices;
	}
}

// Remove AudioDevice objects from the list of the mixer (by object reference)
void chip::AudioEffect::removeAudioDevice(AudioDevice* audioObject) {
	for (int i = 0; i < numAudioDevices; i++) {
		if (audioDeviceList->at(i) == audioObject) {
			audioDeviceList->erase(audioDeviceList->begin() + i);
		}
	}
	numAudioDevices = audioDeviceList->size();
}

// Remove IAudio objects from the list of the mixer (by location)
void chip::AudioEffect::removeAudioDevice(int loc) {
	audioDeviceList->erase(audioDeviceList->begin() + loc);
	numAudioDevices = audioDeviceList->size();
}

// Remove all objects from mixer
void chip::AudioEffect::removeAllAudioDevices() {
	while (numAudioDevices() > 0) {
		removeAudioDevice(0);
	}
}

// Returns the number of objects in this mixer
int chip::AudioEffect::getNumAudioDevices() {
	return numAudioDevices;
}

// Resize the buffer of the mixer
void chip::AudioEffect::resizeBuffer(int newSize) {
// Free the current buffer memory
	free(buffer);

// Reset the buffer size
	bufferSize = newSize;

// reallocate memory
	buffer = new float[bufferSize];

// Resize all child buffer sizes
	for (int i = 0; i < numAudioDevices; i++) {
		(*audioDeviceList)[i]->resizeBuffer(newSize);
	}
}
