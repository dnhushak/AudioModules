#pragma once
#include "PolyVoice.hpp"
#include "Mixer.hpp"
#include "AudioDevice.hpp"
#include "MIDIDevice.hpp"
#include "ConnectableDevice.hpp"
#include "Voice.hpp"
#include "Gain.hpp"
#include <vector>
#include <iostream>
#include <math.h>
#include <AudioUtils.hpp>
#include <unistd.h>

namespace audio {
	enum sustain_t {
		PEDALUP, PEDALDOWN
	};

	class Module: public midi::MIDIDevice,
			public AudioDevice,
			public device::ConnectableDevice<AudioDevice> {
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
