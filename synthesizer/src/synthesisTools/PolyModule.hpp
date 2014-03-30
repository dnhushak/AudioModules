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
	class PolyModule: public AudioEffect {
		public:
			PolyModule(int, int, int);

			float * advance(int);

			virtual void setVoice(Voice * voice);

			// Activate and release PolyVoices
			void activatePolyVoice(int note);
			void releasePolyVoice(int note);

			// Removes any inactive polyVoices
			void cleanup();

			~PolyModule();

		protected:

			int maxPolyVoices;

			pthread_t cleaner_tid;

			void StartCleaner();

			static void * Cleaner(void * args);

			/*** Voice ***/
			Voice * voice;

			Mixer * polyMixer;


	};
}
