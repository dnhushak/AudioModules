#include "Module.hpp"

chip::Module::Module(int initBufferSize, int initSampleRate) {

	//TODO: Implement glissando and arpeggiation - split into separate AudioEffect modules, and reroute the
	// moduleGain inputs to the gliss/arpegg/polyMixer outputs
	bufferSize = initBufferSize;
	sampleRate = initSampleRate;
	buffer = new float[bufferSize];
	polyMixer = new Mixer(bufferSize, sampleRate);
	polyMixer->setAudioDeviceList(audioDeviceList);
	moduleGain = new Gain(bufferSize, sampleRate);
	moduleGain->addAudioDevice(polyMixer);

	voice = new Voice;

	arp_en = false;
	gliss_en = false;
	arpTime = 100;
	glissTime = 1000;

	numActive = 0;

	toDelete = new std::vector<AudioDevice *>;
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
			if (message->data2 == 127) {
				std::cout << "Number of polyvoices in mixer:           "
						<< polyMixer->getNumAudioDevices() << "\n";
				std::cout << "Number of polyvoices in polyvoice array: "
						<< numAudioDevices << "\n";
			}
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
	cleanup();
}

void chip::Module::setVoice(Voice * newVoice) {
	voice = newVoice;
	arp_en = voice->arp_en;
	gliss_en = voice->gliss_en;
	arpTime = voice->arpTime;
	glissTime = voice->glissTime;
	moduleGain->setGain(voice->volume);
}

float * chip::Module::advance(int numSamples) {
	return moduleGain->advance(numSamples);
}

void chip::Module::activatePolyVoice(int note) {
	// Check for existing note in the active polyvoices
	if (numAudioDevices > 0) {
		for (audIter = audioDeviceList->begin();
				audIter != audioDeviceList->end(); ++audIter) {

			// Notation is a little wonkyhere, but we want to access the note
			// So, we need to access the AudioObject in the iterator (*audioDeviceIterator)
			// And then downcast it to PolyVoice (PolyVoice*)
			if (((PolyVoice*) (*audIter))->getNote() == note) {
				// Restart the polyVoice
				((PolyVoice*) (*audIter))->startPolyVoice(note);
				return;
			}
		}
	}

	// If polyVoice with that note wasn't found, Create new polyvoice, and set its parameters
	PolyVoice * newPolyVoice = new PolyVoice(bufferSize, sampleRate);
	newPolyVoice->setVoice(voice);
	newPolyVoice->startPolyVoice(note);

	// Add new polyvoice to the device list
	audioDeviceList->push_front(newPolyVoice);
	numAudioDevices++;
}

void chip::Module::releasePolyVoice(int note) {
	// Release the polyVoice
	for (audIter = audioDeviceList->begin(); audIter != audioDeviceList->end();
			++audIter) {
		if (((PolyVoice*) (*audIter))->getNote() == note) {
			((PolyVoice*) (*audIter))->releasePolyVoice();
			return;
		}
	}
}

void chip::Module::cleanup() {
	// Start with a fresh list of things to delete
	toDelete->clear();

	// Find all inactive polyVoices and put them in the list
	for (audIter = audioDeviceList->begin(); audIter != audioDeviceList->end();
			++audIter) {
		// Grab its pointer before we remove it from the device list
		if ((*audIter)->getState() == INACTIVE) {
			toDelete->push_back(*audIter);
		}
	}

	// Remove everything in the toDelete list from the device list, and call its deconstructor
	for (int i = 0; i < toDelete->size(); i++) {
		audioDeviceList->remove(toDelete->at(i));
		// Free memory
		delete (toDelete->at(i));
	}
	numAudioDevices = audioDeviceList->size();
}
