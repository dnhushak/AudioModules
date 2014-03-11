#include "Module.hpp"

chip::Module::Module(int initBufferSize, int initSampleRate) {

	//TODO: Implement glissando and arpeggiation - split into separate AudioEffect modules, and reroute the
	// moduleGain inputs to the gliss/arpegg/polyMixer outputs
	bufferSize = initBufferSize;
	sampleRate = initSampleRate;
	buffer = new float[bufferSize];
	//instantiates "bucket" of polyvoices
	audioDeviceList = new std::vector<PolyVoice *>(0);
	printf("0\n");
	polyMixer = new Mixer(bufferSize, sampleRate);
	printf(".5\n");
	moduleGain = new Gain(bufferSize, sampleRate);
	printf(".7\n");
	moduleGain->addAudioDevice(polyMixer);

	voice = new Voice;

	arp_en = false;
	gliss_en = false;
	arpTime = 100;
	glissTime = 1000;
}

void chip::Module::affect(MIDIMessage * message) {
	switch (message->type) {
		case 0b1000:
			// Note Off
			releasePolyVoice(message->data1);
			break;
		case 0b1001:
			// Note On
			if (message->data2 == 0) {
				releasePolyVoice(message->data1);
			} else {
				activatePolyVoice(message->data1);
			}
			break;
		case 0b1011:
			// CC
			break;
		case 0b1100:
			// Program Change
			break;
		case 0b1101:
			// Aftertouch (Monophonic)
			break;
		case 0b1110:
			// Pitch Bend
			break;
		case 0b1111:
			// System Message
			break;
		default:
			break;
	}
}

void chip::Module::setVoice(Voice * newVoice) {
	voice = newVoice;
	arp_en = voice->arp_en;
	gliss_en = voice->gliss_en;
	arpTime = voice->arpTime;
	glissTime = voice->glissTime;
	//moduleGain->setGain(voice->volume);
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
	polyMixer->addAudioDevice(newPolyVoice);
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
		if (audioDeviceList->at(i)->getState() == INACTIVE) {
			delete audioDeviceList->at(i);
			audioDeviceList->erase(audioDeviceList->begin() + i);
			polyMixer->removeAudioDevice(i);
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
