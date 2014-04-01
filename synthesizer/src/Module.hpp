#pragma once
#include "PolyVoice.hpp"
#include "Mixer.hpp"
#include "AudioEffect.hpp"
#include "MIDIDevice.hpp"
#include "Voice.hpp"
#include "Gain.hpp"
#include <vector>
#include <iostream>
#include <math.h>
#include <AudioUtils.hpp>
#include <unistd.h>

namespace synth {
	enum sustain_t {
		PEDALUP, PEDALDOWN
	};

	class Module: public MIDIDevice, public AudioEffect {
		public:
			Module(int, int);

			float * advance(int);

			void affect(MIDIMessage *);

			void setVoice(Voice * voice);

			// Activate and release PolyVoices
			void activatePolyVoice(int note);
			void releasePolyVoice(int note);

			// Removes any inactive polyVoices
			void cleanup();

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
			synth::Mixer * polyMixer;
			synth::Gain * ModuleGain;

	};
}
