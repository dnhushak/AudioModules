#include "Module.hpp"

chip::Module::Module() {
	Module(new Voice(1, 1, 0.5, 1, SQUARE, 0, 0, 0));
	mixedFinal = new std::vector<float>(BUFFER_SIZE, 0.0);
	temp = new std::vector<float>(BUFFER_SIZE, 0.0);
	polyvoices = new std::vector<chip::PolyVoice>(0);
}

chip::Module::Module(Voice* voice) {
	//constructor
	this->next = 0;

	//initializes the audio buffer
	mixedFinal = new std::vector<float>(BUFFER_SIZE, 0.0);
	temp = new std::vector<float>(BUFFER_SIZE, 0.0);

	//instantiates "bucket" of polyvoices
	this->polyvoices = new std::vector<chip::PolyVoice>(0);

	// Create the voice for this module
	this->voice = voice;

	// Set arpeggiation and glissando
	this->arpeggio = false;
	this->arpsamples = 1000;
	this->arpcount = 0;
	this->arpnote = 0;

	// Set the volume
	this->volume = 0.4;

//	// Create the 127 polyvoices for the specific module and adds them to the bucket of polyvoices for that module
//	for (int i = 0; i < NUM_POLYVOICES; i++) {
//		chip::PolyVoice* polyvoice = new chip::PolyVoice();
//		polyvoices->push_back(*polyvoice);
//	}

// Set the glissando PolyVoice
	glissando = false;
	glissSamples = 10000;
	glissCount = 0;
	freqSlope = 0.0;
	glissNote = new chip::PolyVoice();
	glissNote->phase = 0.0;
	glissNote->frequency = 0.0;
	glissNote->state = SUSTAIN;
	glissNote->setVoice(voice->getAttack(), voice->getDecay(),
			voice->getSustain(), voice->getRelease(), voice->getWaveType(),
			voice->getVibAmp(), voice->getVibPeriod(), voice->getVibDelay());

	glissEnd = 0.0;
	glissStart = 0.0;
}

void chip::Module::setVoice(int attack, int decay, float sustain, int release,
		int waveType, float vibAmp, int vibPeriod, int vibDelay) {
	voice->setAttack(attack);
	voice->setDecay(decay);
	voice->setSustain(sustain);
	voice->setRelease(release);
	voice->setWaveType(waveType);
	voice->setVibAmp(vibAmp);
	voice->setVibPeriod(vibPeriod);
	voice->setVibDelay(vibDelay);

	// Set the glissando voice
	glissNote->setVoice(voice->getAttack(), voice->getDecay(),
			voice->getSustain(), voice->getRelease(), voice->getWaveType(),
			voice->getVibAmp(), voice->getVibPeriod(), voice->getVibDelay());
}

void chip::Module::setVolume(float newVolume) {
	this->volume = newVolume;
}

std::vector<float> * chip::Module::advance(int numSamples) {
	float sample;
	// Clear the audio buffer
	for (int i = 0; i < numSamples; i++) {
		(*mixedFinal)[i] = 0.0;
	}

	// Perform a cleanup of the polyvoices
	cleanup();

	if (arpeggio) {
		// Grab samples only from the current note in the arpeggio.
		// If the arpeggio count reaches the arpeggio sample number, reset the
		// count.
		for (int i = 0; i < numSamples; i++, arpcount++) {
			if (arpcount >= arpsamples) {
				arpcount = 0;

				if (next > 0) {
					arpnote = (arpnote + 1) % polyvoices->size();
				}

			}

			// Sample if there are active polyvoices
			if (next > 0) {
				(*mixedFinal)[i] = (*polyvoices)[arpnote].getSample() * volume;
			}
		}
	}
	// Glissando only happens if glissando is on and two notes are pressed
	else if (glissando && next == 2) {
		for (int i = 0; i < numSamples; i++) {
			// Make sure gliss happens only if the next gliss frequency won't go
			// past the glissEnd frequency
			if (((glissNote->frequency > glissEnd)
					&& (glissNote->frequency + freqSlope >= glissEnd))
					|| ((glissNote->frequency < glissEnd)
							&& (glissNote->frequency + freqSlope <= glissEnd))) {
				glissNote->frequency += freqSlope;
				glissCount++;
			}

			// Sample the gliss note
			(*mixedFinal)[i] = glissNote->getSample() * volume;
		}
	} else {
		// Loop through the notes being played and sample from them
		for (int i = 0; i < polyvoices->size(); i++) {
			for (int j = 0; j < numSamples; j++) {
				sample = (*polyvoices)[i].getSample();

				//sum each advanced IAudio to the master mixed vector
				(*mixedFinal)[j] = (*mixedFinal)[j] + (sample * volume);
			}
		}
	}

	return mixedFinal; //the final, "synthesized" list
}

void chip::Module::activatePolyVoice(int note) {

	// Reset the arpeggio
	arpnote = 0;

	// Reset glissando
	glissCount = 0;

	// If there already exists a note in the active polyvoices, reset the
	// PolyVoice to attack. Otherwise activate the polyvoice at the next index.
	for (int i = 0; i < polyvoices->size(); i++) {
		if ((*polyvoices)[i].note == note) {
			(*polyvoices)[i].phase = 0.0;
			(*polyvoices)[i].state = ATTACK;
			return;
		}
	}

	// Create new polyvoice, and set its parameters
	PolyVoice * newPolyVoice = new PolyVoice();
	(*newPolyVoice).note = note;
	(*newPolyVoice).phase = 0.0;
	(*newPolyVoice).frequency = MtoF(note);
	(*newPolyVoice).state = ATTACK;
	(*newPolyVoice).setVoice(voice->getAttack(), voice->getDecay(),
			voice->getSustain(), voice->getRelease(), voice->getWaveType(),
			voice->getVibAmp(), voice->getVibPeriod(), voice->getVibDelay());

	//Insert the new polyvoice
	polyvoices->push_back(*newPolyVoice);

	// Update the recent note queue
	glissStart = glissEnd;
	glissEnd = (*newPolyVoice).frequency;

	// Set where glissando will start
	glissNote->frequency = glissStart;

	// Update the frequency slope for gliss if glissando state will be entered
	if (polyvoices->size() == 2) {
		freqSlope = (glissEnd - glissStart) / glissSamples;
	}
}

void chip::Module::releasePolyVoice(int note) {
	// Find a matching note and swap it with the last active polyvoice (next - 1).
	// By setting the to-be-deactivated polyvoice to the last active polyvoice and
	// deactivating the last active polyvoice, we are swapping the two.

	for (int i = 0; i < polyvoices->size(); i++) {
		if ((*polyvoices)[i].note == note) {
			if (arpeggio || glissando) {
				(*polyvoices)[i].state = CLEANUP;
			} else {
				(*polyvoices)[i].releasePolyVoice();
			}

			break;
		}
	}

}

void chip::Module::cleanup() {
	// Remove all polyvoices in cleanup state
	for (int i = 0; i < polyvoices->size(); i++) {
		if ((*polyvoices)[i].state == CLEANUP) {

			polyvoices->erase(polyvoices->begin() + i);
			arpnote = 0;
		}
	}
}

//Midi Note to Frequency
float chip::Module::MtoF(int note) {
	return pow(2, (((float) note - 69) / 12.0)) * 440.0;
}

void chip::Module::printPolyVoices() {
	for (int i = 0; i < polyvoices->size(); i++) {
		std::cout << "polyvoice" << i << ": " << (*polyvoices)[i].note << "\n";
	}

	std::cout << "DONE\n\n";
}
