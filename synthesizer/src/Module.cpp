#include "Module.hpp"

namespace synth {

	Module::Module() {
		//TODO: Implement glissando and arpeggiation - split into separate AudioEffect Modules, and reroute the
		// ModuleGain inputs to the gliss/arpegg/polyMixer outputs
		polyGain.addDevice(&polyMixer);
		state = INACTIVE;
		voice = NULL;
		cleaner_tid = NULL;
		arp_en = false;
		gliss_en = false;
		arpTime = 100;
		glissTime = 1000;
		setMaxNumDevices(10);
		sustain = PEDALUP;
		pthread_mutex_init(&listLock, NULL);

	}

	void Module::affect(MIDIMessage * message) {
		switch (message->statusType) {
			case NOTEOFF:
				// Note Off
				releasePolyVoice(message->data1);
				break;
			case NOTEON:
				// Note On
				if (message->data2 == 0) {
					releasePolyVoice(message->data1);
				} else {
					activatePolyVoice(message->data1);
				}
				break;
			case CC:
				if (message->data1 == 64) {
					if (message->data2 == 0) {
						sustain = PEDALUP;
					} else {
						sustain = PEDALDOWN;
					}
				}
				break;
			case PROGRAM:
				// Program Change
				break;
			case POLYTOUCH:
				// Aftertouch (Monophonic)
				break;
			case PITCHBEND:
				// Pitch Bend
				break;
			case SYSTEM:
				// System Message
				break;
			default:
				break;
		}
	}

	void Module::setVoice(Voice * newVoice) {
		voice = newVoice;
		arp_en = voice->arp_en;
		gliss_en = voice->gliss_en;
		arpTime = voice->arpTime;
		glissTime = voice->glissTime;
		polyGain.setGain(voice->volume);
	}

	sample_t * Module::advance(int numSamples) {
		lockList();
		buffer = polyGain.advance(numSamples);
		unlockList();
		return buffer;
	}

	void Module::activatePolyVoice(int note) {
		// Start the cleaner thread if currently inactive
		if (state == INACTIVE) {
			state = ACTIVE;
			StartCleaner();
		}

		// Check for existing note in the active polyvoices
		if (!isEmpty()) {
			lockList();
			deviceIter = polyMixer.begin();
			// Go from beginning to end of the list
			while (deviceIter != polyMixer.end()) {

				// If the polyvoice has the same note as the new incoming note...
				if (((PolyVoice*) (*deviceIter))->getNote() == note) {
					// ... Restart the polyVoice, unlock and return
					((PolyVoice*) (*deviceIter))->startPolyVoice(note);
					unlockList();
					return;
				} else {
					// ...Or just advance the iterator
					++deviceIter;
				}
			}
			unlockList();
		}

		if (hasSpace()) {
			// If polyVoice with that note wasn't found, Create new polyvoice, and set its parameters
			PolyVoice * newPolyVoice = new PolyVoice;
			newPolyVoice->setVoice(voice);
			newPolyVoice->startPolyVoice(note);
			lockList();
			// Add new polyvoice to the device list
			polyMixer.addDevice(newPolyVoice);
			unlockList();
		}
	}

	void Module::releasePolyVoice(int note) {
		// Release the polyVoice
		lockList();
		deviceIter = polyMixer.begin();
		// Go from beginning to end of the list
		while (deviceIter != polyMixer.end()) {

			// Find the polyvoice with the given note...
			if (((PolyVoice*) (*deviceIter))->getNote() == note) {
				// ... Release the polyVoice, unlock and return
				((PolyVoice*) (*deviceIter))->releasePolyVoice();
				unlockList();
				return;
			} else {
				// ...Or just advance the iterator
				++deviceIter;
			}
		}
		unlockList();
	}

	void Module::cleanup() {
		// Lock the list to prevent data races

		lockList();
		deviceIter = polyMixer.begin();

		// Go from beginning to end of the list
		while (deviceIter != polyMixer.end()) {
			// If the polyvoice is inactive...
			if ((*deviceIter)->getState() == INACTIVE) {
				PolyVoice * toErase = (PolyVoice*) *deviceIter;
				// ... Erase it and set the iterator to the next item
				++deviceIter;
				// This is runtime O(n^2) right now...
				polyMixer.removeDevice(toErase);
				delete toErase;
			} else {
				// Or just advance the iterator
				++deviceIter;
			}
		}
		unlockList();

		if (polyMixer.isEmpty()) {
			// Deactivate the Module
			state = INACTIVE;
		}

	}

	void Module::StartCleaner() {
		pthread_create(&cleaner_tid, NULL, Cleaner, (void *) this);
	}

	void * Module::Cleaner(void * args) {
		Module * mod = (Module *) args;
		while (mod->getState() == ACTIVE) {
			usleep(1000);
			mod->cleanup();
		}
		return NULL;
	}

	void Module::lockList() {
		pthread_mutex_lock(&listLock);
	}

	void Module::unlockList() {
		pthread_mutex_unlock(&listLock);
	}

	Module::~Module() {
		pthread_join(cleaner_tid, NULL);
	}
}
