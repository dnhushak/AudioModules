#ifndef OSCILLATOR_h_
#define OSCILLATOR_h_

#include "AudioDevice.h"
#include "Wavetable.h"

//For phase register maximum
#include <limits.h>
//For the log2 in the table location truncation
#include <math.h>

namespace audio {
	class Oscillator: public AudioDevice {
		public:
			// Constructor
			Oscillator();

			virtual Oscillator * clone();

			// Advance by a given number of samples (in this case summing all in the AudioList
			sample_t * advance();

			// Based on the frequency and sample rate, determine how much to advance the phase register
			void setBaseFrequency(float);

			// Return the oscillator's current frequency
			float getFrequency();

			// Sets the pointer to the wavetable
			void setWavetable(Wavetable *);

		protected:

			void calcStepSize(float);

			// Current phase of the oscillator
			unsigned int phase;

			// The maximum value of the phase register
			// !! If you change the type of phase register,
			// also change this value according to limits.h
			unsigned int phaseMax;

			// The truncated current phase, used to access a wavetable index
			unsigned int phaseTruncated;

			// The amount to truncate the phase by every step increase
			int phaseTruncateAmt;

			// The amount to increase the phase by every sample
			unsigned int stepSize;

			// The current frequency of the oscillator
			float frequency;

			Wavetable * wavetable;
	};
}

#endif //OSCILLATOR_h_
