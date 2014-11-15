#pragma once
#include "AudioDevice.hpp"
#include "Wavetable.hpp"
#include "AudioUtils.hpp"
#include "Oscillator.hpp"
#include "Envelope.hpp"
#include "Voice.hpp"
#include "Ramp.hpp"
#include <vector>
#include <iostream>

namespace audio {
	class PolyVoice: public AudioDevice {
		public:
			// Constructor
			PolyVoice();

			// Advance for callback
			sample_t * advance();

			// Return the current state of the polyvoice
			device::devState_t getState();

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

			~PolyVoice();

		private:

			// Main oscillator
			Oscillator osc;
			// Vibrato oscillator
			Oscillator vib;
			// Main Envelope
			Envelope osc_env;
			// Vibrato Ramp
			Ramp vib_ramp;
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
