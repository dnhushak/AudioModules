#ifndef OSCILLATOR_h_
#define OSCILLATOR_h_

#include "AudioDevice.h"
#include "Wavetable.h"
#include "Alterable.h"

//For phase register maximum
#include <limits.h>
//For the log2 in the table location truncation
#include <math.h>

namespace audio {
	using namespace device;
	using namespace std;

	class Oscillator: public Alterable<AudioDevice> {
		public:
			// Constructor
			Oscillator();

			virtual Oscillator * clone();

			void alter(int paramNum, Parameter p);

			// Advance by a given number of samples (in this case summing all in the AudioList
			sample_t * advance();

			virtual void process(const sample_t *inBuffer, sample_t *outBuffer,
								int samplesToProcess, int numInChannels);

			void setBaseFrequency(float);

			/**
			 * Sets the oscillator's frequency based on a MIDI note
			 * @param MIDINote MIDI note from 0 to 127
			 */
			void setBaseFrequencyMIDI(int);

			// Return the oscillator's current frequency
			float getFrequency();

			// Sets the pointer to the wavetable
			void setWavetable(Wavetable *);

		protected:

			// Based on the frequency and sample rate, determine how much to advance the phase register
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
