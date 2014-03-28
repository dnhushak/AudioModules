#include "Module.hpp"

synth::Module::Module(int initBufferSize, int initSampleRate) {

	//TODO: Implement glissando and arpeggiation - split into separate AudioEffect modules, and reroute the
	// moduleGain inputs to the gliss/arpegg/polyMixer outputs

	resizeBuffer(initBufferSize);
	changeSampleRate(initSampleRate);

	polyMixer = new Mixer(bufferSize, sampleRate);
	// We use the Module's device list so we can access and edit later
	polyMixer->setAudioDeviceList(audioDeviceList);
	moduleGain = new Gain(bufferSize, sampleRate);
	moduleGain->addAudioDevice(polyMixer);
	state = INACTIVE;
	voice = NULL;
	cleaner_tid = NULL;
	arp_en = false;
	gliss_en = false;
	arpTime = 100;
	glissTime = 1000;

	numAudioDevices = 0;

	toDelete = new std::vector<AudioDevice *>;
}

void synth::Module::affect(MIDIMessage * message) {
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

void synth::Module::setVoice(Voice * newVoice) {
	voice = newVoice;
	arp_en = voice->arp_en;
	gliss_en = voice->gliss_en;
	arpTime = voice->arpTime;
	glissTime = voice->glissTime;
	moduleGain->setGain(voice->volume);
}

float * synth::Module::advance(int numSamples) {
	lockList();
	buffer = moduleGain->advance(numSamples);
	unlockList();
	return buffer;
}

void synth::Module::activatePolyVoice(int note) {

	// Start the cleaner thread if currently inactive
	if (state == INACTIVE) {
		state = ACTIVE;
		StartCleaner();
	}

//	// Check for existing note in the active polyvoices
//	if (numAudioDevices > 0) {
//		lockList();
//		for (audIter = audioDeviceList->begin();
//				audIter != audioDeviceList->end(); ++audIter) {
//
//			// Notation is a little wonky here, but we want to access the note
//			// So, we need to access the AudioObject in the iterator (*audioDeviceIterator)
//			// And then downcast it to PolyVoice (PolyVoice*)
//			if (((PolyVoice*) (*audIter))->getNote() == note) {
//				// Restart the polyVoice
//				((PolyVoice*) (*audIter))->startPolyVoice(note);
//				return;
//			}
//
//			printf("In Update\n");
//		}
//		unlockList();
//	}

	// Check for existing note in the active polyvoices
	if (numAudioDevices > 0) {
		lockList();
		audIter = audioDeviceList->begin();
		// Go from beginning to end of the list
		while (audIter != audioDeviceList->end()) {

			// If the polyvoice has the same note as the new incoming note...
			if (((PolyVoice*) (*audIter))->getNote() == note) {

				// ... Restart the polyVoice, unlock and return
				((PolyVoice*) (*audIter))->startPolyVoice(note);
				unlockList();
				return;
			} else {
				// ...Or just advance the iterator
				++audIter;
			}
		}
		unlockList();
	}

	if (numAudioDevices < 10) {
		// If polyVoice with that note wasn't found, Create new polyvoice, and set its parameters
		PolyVoice * newPolyVoice = new PolyVoice(bufferSize, sampleRate);
		newPolyVoice->setVoice(voice);
		newPolyVoice->startPolyVoice(note);

		lockList();
		// Add new polyvoice to the device list
		audioDeviceList->push_front(newPolyVoice);
		numAudioDevices = audioDeviceList->size();
		unlockList();
	}
}

void synth::Module::releasePolyVoice(int note) {
	// Release the polyVoice
	lockList();
	audIter = audioDeviceList->begin();
	// Go from beginning to end of the list
	while (audIter != audioDeviceList->end()) {

		// If the polyvoice has the same note as the new incoming note...
		if (((PolyVoice*) (*audIter))->getNote() == note) {
			// ... Release the polyVoice, unlock and return
			((PolyVoice*) (*audIter))->releasePolyVoice();
			unlockList();
			return;
		} else {
			// ...Or just advance the iterator
			++audIter;
		}
	}
	unlockList();
}

void synth::Module::cleanup() {
	// Lock the list to prevent data races
	lockList();
	audIter = audioDeviceList->begin();

	// Go from beginning to end of the list
	while (audIter != audioDeviceList->end()) {
		// If the polyvoice is inactive...
		if ((*audIter)->getState() == INACTIVE) {
			// ... Erase it and set the iterator to the next item
			audIter = audioDeviceList->erase(audIter);
		} else {
			// Or just advance the iterator
			++audIter;
		}
	}
	// Recound the number of devices
	numAudioDevices = audioDeviceList->size();
	unlockList();

	if (numAudioDevices == 0) {
		// Deactivate the Module
		state = INACTIVE;
	}

}

void synth::Module::StartCleaner() {
	pthread_create(&cleaner_tid, NULL, Module::Cleaner, this);
}

void * synth::Module::Cleaner(void * args) {
	Module * mod = (Module *) args;
	while (mod->getState() == ACTIVE) {
		mod->cleanup();
		usleep(10000);
	}
	return NULL;
}

synth::Module::~Module() {
	pthread_join(cleaner_tid, NULL);
	delete polyMixer;
	delete moduleGain;
	delete toDelete;
}
