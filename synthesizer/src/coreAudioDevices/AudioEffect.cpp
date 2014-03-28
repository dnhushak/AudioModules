#include "AudioEffect.hpp"

namespace synth {
	AudioEffect::AudioEffect() {
		audioDeviceList = new std::list<AudioDevice*>;
		maxNumAudioDevices = -1;
		numAudioDevices = 0;
		audIter = audioDeviceList->begin();
		audCallbackIter = audioDeviceList->begin();
		pthread_mutex_init(&listLock, NULL);
	}

	void AudioEffect::cleanup() {
		for (audIter = audioDeviceList->begin();
				audIter != audioDeviceList->end(); ++audIter) {
			(*audIter)->cleanup();
		}
	}

// Add another AudioDevice object to be mixed
	void AudioEffect::addAudioDevice(AudioDevice * audioObject) {
		// Ignore if at maximum value or no maximum
		if (numAudioDevices < maxNumAudioDevices || maxNumAudioDevices == -1) {
			audioDeviceList->push_front(audioObject);
			numAudioDevices++;
		}
	}

//// Add a list of AudioDevices to be mixed
//void chip::AudioEffect::addAudioDevices(
//		std::list<AudioDevice*> * audioObjects) {
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
	void AudioEffect::setAudioDeviceList(
			std::list<AudioDevice*> * audioObjects) {

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

		numAudioDevices = audioDeviceList->size();
	}

	// Remove AudioDevice objects from the list of the mixer (by object reference)
	// Note also calls destructor of device we are removing
	void AudioEffect::removeAudioDevice(AudioDevice* audioObject) {
		audioDeviceList->remove(audioObject);
		numAudioDevices = audioDeviceList->size();
	}

	// Remove IAudio objects from the list of the mixer (by location)
	void AudioEffect::removeAudioDevice(int loc) {
		//	// TODO: fix this
		//	audioDeviceList->erase(audioDeviceList->begin() + loc);
		//	numAudioDevices = audioDeviceList->size();
	}

	// Remove all objects from mixer
	void AudioEffect::removeAllAudioDevices() {
		audioDeviceList->clear();
		numAudioDevices = audioDeviceList->size();
	}

	// Returns the number of objects in this mixer
	int AudioEffect::getNumAudioDevices() {
		return audioDeviceList->size();
	}

	// Resize the buffer of the mixer
	void AudioEffect::resizeBuffer(int newSize) {
		// Free the current buffer memory
		free(buffer);

		// Reset the buffer size
		bufferSize = newSize;

		// reallocate memory
		buffer = new float[bufferSize];

		// Resize all child buffer sizes
		for (audIter = audioDeviceList->begin();
				audIter != audioDeviceList->end(); ++audIter) {
			(*audIter)->resizeBuffer(bufferSize);
		}
	}

	void AudioEffect::lockList() {
		pthread_mutex_lock(&listLock);
	}

	void AudioEffect::unlockList() {
		pthread_mutex_unlock(&listLock);
	}

	AudioEffect::~AudioEffect() {
		delete audioDeviceList;
	}
}
