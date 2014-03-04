#include "Mixer.hpp"

chip::Mixer::Mixer() {
	//constructor
	audioList = new std::vector<IAudio*>(0);

	// Initialize the audio buffer
	mixed = new std::vector<float>(BUFFER_SIZE, 0.0);
	temp = new std::vector<float>(BUFFER_SIZE, 0.0);
}

std::vector<float> * chip::Mixer::advance(int numSamples) {
	//the 0th elements are all added together, the 1st elements, 2nd, all the way to the 
	//nth elements and the result is returned (aka - move along the sound wave)

	mixed->clear();
	for (unsigned int i = 0; i < audioList->size(); i++) {
		// For each IAudio in audioList, advance
		temp = (*audioList)[i]->advance(numSamples);
		for (int j = 0; j < numSamples; j++) {

			// Sum each advanced IAudio to the master mixed vector
			(*mixed)[j] += (*temp)[j];

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

