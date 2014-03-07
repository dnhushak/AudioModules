#include "AudioEffect.hpp"

// Add another AudioDevice object to be mixed
void chip::AudioEffect::addObject(AudioDevice* audioObject) {
	audioDeviceList->push_back(audioObject);
}

// Add a vector of AudioDevices to be mixed
void chip::AudioEffect::addObjects(std::vector<AudioDevice*> * audioObjects) {
	audioDeviceList->insert(audioDeviceList->end(), audioObjects->begin(),
			audioObjects->end());
}

// Add a vector of AudioDevices to be mixed
void chip::AudioEffect::setDeviceList(
		std::vector<AudioDevice*> * audioObjects) {
	audioDeviceList = audioObjects;
}

// Remove IAudio objects from the list of the mixer (by object reference)
void chip::AudioEffect::removeObject(AudioDevice* audioObject) {
	for (int i = 0; i < audioDeviceList->size(); i++) {
		if (audioDeviceList->at(i) == audioObject) {
			audioDeviceList->erase(audioDeviceList->begin() + i);
		}
	}
}

// Remove IAudio objects from the list of the mixer (by location)
void chip::AudioEffect::removeObject(int loc) {
	audioDeviceList->erase(audioDeviceList->begin() + loc);
}

// Remove all objects from mixer
void chip::AudioEffect::removeAllObjects() {
	while (getNumObjects() > 0) {
		removeObject(0);
	}
}

// Returns the number of objects in this mixer
int chip::AudioEffect::getNumObjects() {
	return audioDeviceList->size();
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
	for (int i = 0; i < audioDeviceList->size(); i++) {
		(*audioDeviceList)[i]->resizeBuffer(newSize);
	}
}
