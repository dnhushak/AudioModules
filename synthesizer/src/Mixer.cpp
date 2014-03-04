#include "Mixer.hpp"

chip::Mixer::Mixer() {
	//constructor
	audioList = new std::vector<IAudio*>(0);

	// Initialize the audio buffer
	mixedFinal = new std::vector<float>(BUFFER_SIZE, 0.0);
	temp = new std::vector<float>(BUFFER_SIZE, 0.0);
}

std::vector<float> * chip::Mixer::advance(int numSamples) {
	//the 0th elements are all added together, the 1st elements, 2nd, all the way to the 
	//nth elements and the result is returned (aka - move along the sound wave)

	for (unsigned int i = 0; i < audioList->size(); i++) {
		// For each IAudio in audioList, advance
		temp = (*audioList)[i]->advance(numSamples);
		for (int j = 0; j < numSamples; j++) {

			// Zero out our audio buffer on the first iteration so the previous
			// audio data is cleared.
			if (i == 0) {
				(*mixedFinal)[j] = 0.0;
			}

			// Sum each advanced IAudio to the master mixed vector
			(*mixedFinal)[j] += (*temp)[j];

			// Clear the temp variable
			(*temp)[j] = 0.0;
		}
	}
	temp->clear();
	//delete temp;

	return mixedFinal; //the final, "synthesized" list
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

