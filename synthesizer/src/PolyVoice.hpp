#pragma once
#include "AudioDevice.hpp"
#include "Wavetables.hpp"
#include "chiputil.hpp"
#include "Oscillator.hpp"
#include "Envelope.hpp"
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

		private:
			// Current state of polyvoices
			devState_t state = ACTIVE;

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
			bool vib_en = true;
	};

}
