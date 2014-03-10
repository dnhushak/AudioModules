#pragma once
#include "AudioEffect.hpp"
#include "Wavetable.hpp"
#include <vector>
#include <iostream>
#include <limits.h>
#include <math.h>

namespace chip {
	class Oscillator: public AudioDevice {
		public:
			// Constructor
			Oscillator(int, int);

			// Advance by a given number of samples (in this case summing all in the AudioList
			float * advance(int);

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
			int phaseTruncated;

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
