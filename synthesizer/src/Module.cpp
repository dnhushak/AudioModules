#include "Module.hpp"

void chip::Module::Module(int initBufferSize, int initSampleRate) {

	//TODO: Implement glissando and arpeggiation - split into separate AudioEffect modules, and reroute the
	// moduleGain inputs to the gliss/arpegg/polyMixer outputs

	bufferSize = initBufferSize;
	sampleRate = initSampleRate;
	buffer = new float[bufferSize];

	//instantiates "bucket" of polyvoices
	audioDeviceList = new std::vector<chip::PolyVoice *>(0);
	polyMixer = new Mixer(bufferSize, sampleRate);
	moduleGain = new Gain(bufferSize, sampleRate);

	// Point the polyMixer device list to the module's device list
	// Allows to update/erase only once, and use only one memory space for all the pointers
	polyMixer->setAudioDeviceList(
			(std::vector<chip::AudioDevice *> *) audioDeviceList);

	// Inputs the polyMixer into the gain module
	moduleGain->addAudioDevice(polyMixer);

}

void chip::Module::setVoice(Voice * newVoice) {
	voice = newVoice;
	arp_en = voice->arp_en;
	gliss_en = voice->gliss_en;
	arpTime = voice->arpTime;
	glissTime = voice->glissTime;
	volume = voice->volume;
}

float * chip::Module::advance(int numSamples) {
	return moduleGain->advance(numSamples);
}

void chip::Module::activatePolyVoice(int note) {
	// Check for existing note in the active polyvoices
	for (int i = 0; i < audioDeviceList->size(); i++) {
		if ((*audioDeviceList)[i]->getNote() == note) {
			// Restart the polyVoice
			(*audioDeviceList)[i]->startPolyVoice(note);
			return;
		}
	}

	// Create new polyvoice, and set its parameters
	PolyVoice * newPolyVoice = new PolyVoice(bufferSize, sampleRate);
	newPolyVoice->setVoice(voice);
	newPolyVoice->startPolyVoice(note);

	// Add new polyvoice to the device list
	audioDeviceList->push_back(newPolyVoice);
}

void chip::Module::releasePolyVoice(int note) {
	// Release the polyVoice
	for (int i = 0; i < audioDeviceList->size(); i++) {
		if ((*audioDeviceList)[i]->getNote() == note) {
			(*audioDeviceList)[i]->releasePolyVoice();
			break;
		}
	}
}

void chip::Module::cleanup() {
	// Remove all polyvoices in cleanup state
	for (int i = 0; i < audioDeviceList->size(); i++) {
		if ((*audioDeviceList)[i]->getState == INACTIVE) {
			audioDeviceList->erase(audioDeviceList->begin() + i);
		}
	}
}

//Midi Note to Frequency
float chip::Module::MtoF(int note) {
	return pow(2, (((float) note - 69) / 12.0)) * 440.0;
}

void chip::Module::printPolyVoices() {
	std::cout << "Allocated PolyVoices: " << this->audioDeviceList->size()
			<< "\n";
	for (int i = 0; i < audioDeviceList->size(); i++) {
		std::cout << "polyvoice" << i << ": "
				<< (*audioDeviceList)[i]->getNote() << "\n";
	}
	std::cout << "***DONE\n";
}
