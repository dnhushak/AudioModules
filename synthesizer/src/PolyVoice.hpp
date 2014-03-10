#pragma once
#include "AudioDevice.hpp"
#include "Wavetable.hpp"
#include "AudioUtils.hpp"
#include "Oscillator.hpp"
#include "Envelope.hpp"
#include "Voice.hpp"
#include <vector>
#include <iostream>

namespace chip {
	class PolyVoice: public AudioDevice {
		public:
			// Constructor
			PolyVoice(int, int);

			// Advance for callback
			float * advance(int);

			// Return the current state of the polyvoice
			devState_t getState();

			// Enable vibrato
			void enableVibrato();

			// Disable vibrato
			void disableVibrato();

			// Start the polyvoice
			void startPolyVoice(int);

			// Release the polyvoice (with envelopes, doesn't necessarily deactivate it)
			void releasePolyVoice();

			// Sets the parameters of the polyvoice
			void setVoice(Voice *);

			// Gets the MIDI note number tied to this polyVoice
			int getNote();

		private:
			// Current state of polyvoice
			devState_t state;

			// Main oscillator
			Oscillator * osc;
			// Vibrato oscillator
			Oscillator * vib;
			// Main Envelope
			Envelope * osc_env;
			// Vibrato Envelope
			Envelope * vib_env;
			// Vibrato multiplier
			float vibmult;
			// Base frequency
			//(also stored in oscillator, but used in the PolyVoice for vibrato calculations)
			float baseFrequency;

			// Current MIDI note of polyVoice
			int note;

			/*** Feature activation/deactivation ***/

			// Vibrato
			bool vib_en;
	};

}
