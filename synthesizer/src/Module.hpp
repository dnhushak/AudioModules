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

namespace chip {
	class Module: public AudioEffect, public MIDIDevice {
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

			//Midi Note to Frequency
			float MtoF(int note);

			void printPolyVoices();

		private:

			/*** Voice ***/
			Voice * voice;

			bool arp_en;
			bool gliss_en;
			int arpTime;
			int glissTime;

			// List of active notes
			std::vector<PolyVoice *> * audioDeviceList;

			chip::Mixer * polyMixer;
			chip::Gain * moduleGain;

	};
}
