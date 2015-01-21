#pragma once
#include "PolyVoice.h"
#include "Mixer.h"
#include "AudioDevice.h"
#include "MIDIDevice.h"
#include "ConnectableDevice.h"
#include "Voice.h"
#include "Gain.h"
#include <vector>
#include <iostream>
#include <math.h>
#include <AudioUtils.h>
#include <unistd.h>

namespace audio {
	enum sustain_t {
		PEDALUP, PEDALDOWN
	};

	class Module: public midi::MIDIDevice,
			public AudioDevice,
			public device::ConnectableDevice<AudioDevice, AudioDevice> {
		public:
			Module();

			sample_t * advance();

			void affect(midi::MIDIMessage *);

			void setVoice(Voice * voice);

			// Activate and release PolyVoices
			void activatePolyVoice(int note);
			void releasePolyVoice(int note);

			// Removes any inactive polyVoices
			void cleanup();

			void lockList();

			void unlockList();

			~Module();

		private:

			pthread_t cleaner_tid;

			void StartCleaner();

			static void * Cleaner(void * args);

			/*** Voice ***/
			Voice * voice;

			bool arp_en;
			bool gliss_en;
			int arpTime;
			int glissTime;
			sustain_t sustain;
			Mixer polyMixer;
			Gain polyGain;

			pthread_mutex_t listLock;
	}
	;

}
