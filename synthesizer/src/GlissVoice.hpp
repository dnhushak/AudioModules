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

namespace synth {
	class GlissVoice: public AudioDevice {
		public:
			// Constructor
			GlissVoice(int, int);

			// Advance for callback
			float * advance(int);

			// Return the current state of the Glissvoice
			devState_t getState();

			// Enable vibrato
			void enableVibrato();

			// Disable vibrato
			void disableVibrato();

			// Start the Glissvoice
			void startGlissVoice(int);

			// Release the Glissvoice (with envelopes, doesn't necessarily deactivate it)
			void releaseGlissVoice();

			// Sets the parameters of the Glissvoice
			void setVoice(synth::Voice *);

			// Gets the MIDI note number tied to this GlissVoice
			int getNote();

			~GlissVoice();

		private:

			// Main oscillator
			Oscillator * osc;
			// Vibrato oscillator
			Oscillator * vib;
			// Main Envelope
			Envelope * osc_env;
			// Vibrato Ramp
			Ramp * vib_ramp;
			// Glissando Ramp
			Ramp * gliss_ramp;
			// Vibrato multiplier
			float vibmult;
			// Base frequency
			//(also stored in oscillator, but used in the GlissVoice for vibrato calculations)
			float baseFrequency;

			// Current MIDI note of GlissVoice
			int note;

			/*** Feature activation/deactivation ***/

			// Vibrato
			bool vib_en;
	};

}
