#pragma once
#include "IAudio.hpp"
#include "chiputil.hpp"
#include <vector>
#include <iostream>

namespace chip {
	class Envelope: public IAudio {
		public:

			// Constructor
			void Envelope(int sampFreq);

			// Gets the current state
			int getState();

			// Releases the Envelope
			void release();

			// Gets and sets the envelope multiplier
			float getEnvmult();
			void setEnvmult(float);

			// Gets and sets the envelope location
			int getEnvloc();
			void setEnvloc(int);

			// Sets the envelope properties
			void setAttack(int);
			void setDecay(int);
			void setSustain(float);
			void setRelease(int);

			// Advance/fill the buffer
			float * advance(int);
		private:
			// Sampling rate of soundcard/ audio process
			int sampFreq;

			// Current state of envelope
			int state;

			// ADSR - A, D, and R are in ms, S is a multiplier scaled from 0.0 - 1.0
			int attack;
			int decay;
			float sustain;
			int release;

			// Envelope multiplier
			float envmult;

			// Envelope location
			int envloc;

			// Length (in samples) of the attack
			int AsampCount;
			// Slope of the attack curve
			float Aslope;

			// Length (in samples) of the decay
			int DsampCount;
			// Slope of the decay curve
			float Dslope;

			// Length (in samples) of the release
			int RsampCount;
			// Slope of the release curve
			float Rslope;
	};
}
