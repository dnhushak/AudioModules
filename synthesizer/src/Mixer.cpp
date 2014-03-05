#include "Mixer.hpp"

chip::Mixer::Mixer(int initSize) {
	//constructor
	audioList = new std::vector<IAudio*>(0);

	bufferSize = initSize;
	// Initialize the audio buffer
	mixed = new float[bufferSize];
	temp = new float[bufferSize];
}

float * chip::Mixer::advance(int numSamples) {

	// Zero out the buffer
	zeroBuffer();

	// Vector sum each element of the list of IAudio compatible objects
	for (unsigned int i = 0; i < audioList->size(); i++) {
		// Fill up a temp buffer for one IAudio object
		temp = (*audioList)[i]->advance(numSamples);

		// Add each element into the mixdown buffer
		for (int j = 0; j < bufferSize; j++) {
			// Sum each advanced IAudio to the master mixed vector
			mixed[j] += temp[j];
		}
	}

	//Pointer to the summed buffer
	return mixed;
}

// Add another IAudio object to be mixed
void chip::Mixer::addObjects(IAudio* audioObject) {
	audioList->push_back(audioObject);
}

// Remove IAudio objects from the list of the mixer (by object reference)
void chip::Mixer::removeObjects(IAudio* audioObject) {
	for (int i = 0; i < audioList->size(); i++) {
		if (audioList->at(i) == audioObject) {
			audioList->erase(audioList->begin() + i);
		}
	}
}

// Remove IAudio objects from the list of the mixer (by location)
void chip::Mixer::removeObjects(int loc) {
	audioList->erase(audioList->begin() + loc);
}

// Resize the buffer of the mixer
void chip::Mixer::resizeBuffer(int newSize) {
	// Free the current buffer memories
	free(temp);
	free(mixed);

	// Reset the buffer size
	bufferSize = newSize;

	// reallocate memory
	mixed = new float[bufferSize];
	temp = new float[bufferSize];
}

// Set every value in buffer to 0
void chip::Mixer::zeroBuffer() {
	for (int i = 0; i < bufferSize; i++) {
		mixed[i] = 0.0;
	}
}

// Remove all objects from mixer
void chip::Mixer::clearMixer() {
	while (getNumObjects()>0){
		removeObjects(0);
	}
}

// Returns the number of objects in this mixer
int chip::Mixer::getNumObjects(){
	return audioList->size();
}

