#include "Module.hpp"

namespace synth {

	Module::Module(int initBufferSize, int initSampleRate) :
			MIDIDevice(), AudioEffect() {

		//TODO: Implement glissando and arpeggiation - split into separate AudioEffect Modules, and reroute the
		// ModuleGain inputs to the gliss/arpegg/polyMixer outputs

		resizeBuffer(initBufferSize);
		changeSampleRate(initSampleRate);

		polyMixer = new Mixer(bufferSize, sampleRate);
		// We use the Module's device list so we can access and edit later
		polyMixer->setAudioDeviceList(audioDeviceList);
		ModuleGain = new Gain(bufferSize, sampleRate);
		ModuleGain->addAudioDevice(polyMixer);
		state = INACTIVE;
		voice = NULL;
		cleaner_tid = NULL;
		arp_en = false;
		gliss_en = false;
		arpTime = 100;
		glissTime = 1000;

		numAudioDevices = 0;
		sustain = PEDALUP;

	}

	void Module::affect(MIDIMessage * message) {
		switch (message->type) {
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
		ModuleGain->setGain(voice->volume);
	}

	float * Module::advance(int numSamples) {
		lockList();
		buffer = ModuleGain->advance(numSamples);
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

	void Module::releasePolyVoice(int note) {
		// Release the polyVoice
		lockList();
		audIter = audioDeviceList->begin();
		// Go from beginning to end of the list
		while (audIter != audioDeviceList->end()) {

			// Find the polyvoice with the given note...
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

	void Module::cleanup() {
		// Lock the list to prevent data races

		lockList();
		audIter = audioDeviceList->begin();

		// Go from beginning to end of the list
		while (audIter != audioDeviceList->end()) {
			// If the polyvoice is inactive...
			if ((*audIter)->getState() == INACTIVE) {
				PolyVoice * toErase = (PolyVoice*) *audIter;
				// ... Erase it and set the iterator to the next item
				audIter = audioDeviceList->erase(audIter);

				//delete toErase;
			} else {
				// Or just advance the iterator
				++audIter;
			}
		}
		// Recount the number of devices
		numAudioDevices = audioDeviceList->size();
		unlockList();

		if (numAudioDevices == 0) {
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
			mod->cleanup();
			usleep(10000);
		}
		return NULL;
	}

	Module::~Module() {
		pthread_join(cleaner_tid, NULL);
		delete polyMixer;
		delete ModuleGain;
	}
}
