#ifndef OSCILLATOR_HPP_
#define OSCILLATOR_HPP_

#include "AudioDevice.hpp"
#include "Wavetable.hpp"

//For phase register maximum
#include <limits.h>
//For the log2 in the table location truncation
#include <math.h>

namespace audio {
	class Oscillator: public AudioDevice {
		public:
			// Constructor
			Oscillator();

			// Advance by a given number of samples (in this case summing all in the AudioList
			sample_t * advance(int);

			// Based on the frequency and sample rate, determine how much to advance the phase register
			void setFrequency(float);

			// Return the oscillator's current frequency
			float getFrequency();

			// Sets the pointer to the wavetable
			void setWavetable(Wavetable *);

		private:
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

			// The scale
			int phaseScale;

			// The amount to increase the phase by every sample
			unsigned int stepSize;

			// The current frequency of the oscillator
			float frequency;

			Wavetable * wavetable;
	};
}

#endif //OSCILLATOR_HPP_
