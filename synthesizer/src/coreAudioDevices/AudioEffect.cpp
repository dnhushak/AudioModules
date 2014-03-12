#include "AudioEffect.hpp"

chip::AudioEffect::AudioEffect() {
	audioDeviceList = new std::forward_list<AudioDevice*>(0);
	maxNumAudioDevices = -1;
	numAudioDevices = 0;
	audIter = audioDeviceList->begin();
}

// Perform cleanup on all devices in audio device list
void chip::AudioEffect::cleanup() {
	if (numAudioDevices > 0) {
		for (audIter = audioDeviceList->begin();
				audIter != audioDeviceList->end(); ++audIter) {
			//(*audIter)->cleanup();
		}
	}
}

// Add another AudioDevice object to be mixed
void chip::AudioEffect::addAudioDevice(AudioDevice * audioObject) {
	// Ignore if at maximum value or no maximum
	if (numAudioDevices < maxNumAudioDevices || maxNumAudioDevices == -1) {
		audioDeviceList->push_front(audioObject);
		numAudioDevices++;
	}
}

//// Add a forward_list of AudioDevices to be mixed
//void chip::AudioEffect::addAudioDevices(
//		std::forward_list<AudioDevice*> * audioObjects) {
//	int numToAdd;
//
//	// If no maximum...
//	if (maxNumAudioDevices == -1) {
//		// Add all objects
//		numToAdd = audioObjects->size();
//	}
//	// ...Or number of devices won't put us at our maximum
//	else if ((numAudioDevices + audioObjects->size()) < maxNumAudioDevices) {
//		// Add all objects
//		numToAdd = audioObjects->size();
//	}
//	// Else add as many as are available
//	else {
//		numToAdd = maxNumAudioDevices - numAudioDevices;
//	}
//
//	audioDeviceList->insert(audioDeviceList->end(), audioObjects->begin(),
//			audioObjects->begin() + numToAdd);
//
//	numAudioDevices = audioDeviceList->size();
//}

// Switch pointer to a new audioDeviceList - Useful if list is managed by external object
void chip::AudioEffect::setAudioDeviceList(
		std::forward_list<AudioDevice*> * audioObjects) {

	// Re-reference
	audioDeviceList = audioObjects;

	//TODO: Fix maximum logic for changing the audioDeviceList
// If the new device list has less than the maximum number of devices, or there is no maximum...
//	if (audioDeviceList->size() < maxNumAudioDevices
//			|| maxNumAudioDevices == -1) {
//		// Then the current number of devices is the size of the new list
//		numAudioDevices = audioDeviceList->size();
//	} else {
//		// Otherwise set to maximum number of devices
//		numAudioDevices = maxNumAudioDevices;
//	}
}

// Remove AudioDevice objects from the list of the mixer (by object reference)
void chip::AudioEffect::removeAudioDevice(AudioDevice* audioObject) {
	// Using an iterator
//	std::forward_list<AudioDevice *>::iterator it = audioDeviceList->begin();
//	while (it < audioDeviceList->end()) {
//		if ((*it) == audioObject) {
//			it = audioDeviceList->erase(it);
//			break;
//		}
//		it++;
//	}
	audioDeviceList->remove(audioObject);
	numAudioDevices--;
}

// Remove IAudio objects from the list of the mixer (by location)
//void chip::AudioEffect::removeAudioDevice(int loc) {
//	audioDeviceList->erase(audioDeviceList->begin() + loc);
//	numAudioDevices = audioDeviceList->size();
//}

// Remove all objects from mixer
void chip::AudioEffect::removeAllAudioDevices() {
	audioDeviceList->clear();
	numAudioDevices = 0;
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
	for (audIter = audioDeviceList->begin(); audIter != audioDeviceList->end();
			++audIter) {
		(*audIter)->resizeBuffer(bufferSize);
	}
}
