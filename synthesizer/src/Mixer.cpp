#include "Mixer.hpp"

chip::Mixer::Mixer(int bufferSize) {
	//constructor
	audioList = new std::vector<IAudio*>(0);

	// Initialize the audio buffer
	mixed = new std::vector<float>(bufferSize, 0.0);
	temp = new std::vector<float>(bufferSize, 0.0);
}

std::vector<float> * chip::Mixer::advance(int numSamples) {

	// Vector sum each element of the list of IAudio compatible objects

	for (unsigned int i = 0; i < audioList->size(); i++) {
		// For each IAudio in audioList, advance
		temp = (*audioList)[i]->advance(numSamples);
		for (int j = 0; j < numSamples; j++) {
			// Sum each advanced IAudio to the master mixed vector
			if (i == 0) {
				*mixed[j] = (*temp)[j];
			} else {
				(*mixed)[j] += (*temp)[j];
			}
		}
	}

	//Pointer to the summed buffer
	return mixed;
}

void chip::Mixer::addObjects(IAudio* audioObject) {
	audioList->push_back(audioObject);
}

void chip::Mixer::removeObjects(IAudio* audioObject) {
	for (int i = 0; i < audioList->size(); i++) {
		if (audioList->at(i) == audioObject) {
			audioList->erase(audioList->begin() + i);
		}
	}
}

void chip::Mixer::removeObjects(int loc) {
	audioList->erase(audioList->begin() + loc);
}

void chip::Mixer::resizeBuffer(int bufferSize){
	temp->resize(bufferSize);
	mixed->resize(bufferSize);
}

