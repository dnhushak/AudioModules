#pragma once
#include "PolyVoice.hpp"
#include "Mixer.hpp"
#include "AudioEffect.hpp"
#include "Voice.hpp"
#include "Gain.hpp"
#include "chiputil.hpp"
#include <vector>
#include <iostream>
#include <math.h>

namespace chip {
	class Module: public AudioEffect {
		public:
			Module(int, int);

			float * advance(int);

			void setVoice(Voice * voice);

			// Activate and release PolyVoices
			void activatePolyVoice(int note);
			void releasePolyVoice(int note);

			// Removes any inactive polyVoices
			void cleanup();

			//Midi Note to Frequency
			float MtoF(int note);

			void printPolyVoices();

		private:

			/*** Voice ***/
			Voice * voice;

			bool arp_en = false;
			bool gliss_en = false;
			int arpTime = 100;
			int glissTime = 1000;
			float volume = -6;

			// List of active notes
			std::vector<PolyVoice *> * audioDeviceList;

			Mixer * polyMixer;
			Gain * moduleGain;

	};
}
