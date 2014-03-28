#include "PolyModule.hpp"
namespace synth {
	PolyModule::PolyModule(int initBufferSize, int initSampleRate) {
		resizeBuffer(initBufferSize);
		changeSampleRate(initSampleRate);

		polyMixer = new Mixer(bufferSize, sampleRate);
		// We use the PolyModule's device list so we can access and edit later
		polyMixer->setAudioDeviceList(audioDeviceList);
		state = INACTIVE;
		voice = NULL;
		cleaner_tid = NULL;
		numAudioDevices = 0;
	}

	void PolyModule::setVoice(Voice * newVoice) {
		voice = newVoice;
	}

	float * PolyModule::advance(int numSamples) {
		lockList();
		buffer = polyMixer->advance(numSamples);
		unlockList();
		return buffer;
	}

	void PolyModule::activatePolyVoice(int note) {

		// Start the cleaner thread if currently inactive
		if (state == INACTIVE) {
			state = ACTIVE;
			StartCleaner();
		}

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

	void PolyModule::releasePolyVoice(int note) {
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

	void PolyModule::cleanup() {
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
			// Deactivate the PolyModule
			state = INACTIVE;
		}

	}

	void PolyModule::StartCleaner() {
		pthread_create(&cleaner_tid, NULL, PolyModule::Cleaner, this);
	}

	void * PolyModule::Cleaner(void * args) {
		PolyModule * mod = (PolyModule *) args;
		while (mod->getState() == ACTIVE) {
			mod->cleanup();
			usleep(10000);
		}
		return NULL;
	}

	PolyModule::~PolyModule() {
		pthread_join(cleaner_tid, NULL);
		delete polyMixer;
	}
}
