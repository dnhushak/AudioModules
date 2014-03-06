#pragma once
#include "IAudio.hpp"
#include "chiputil.hpp"
#include <vector>
#include <iostream>

namespace chip {

	typedef enum envState_t {
		INIT, ATTACK, DECAY, SUSTAIN, RELEASE, DONE,
	} envState_t;
	class Envelope: public IAudio {
		public:
			// Constructor
			void Envelope(int);

			// Advance/fill the buffer
			float * advance(int);

			// Necessary?
//			// Gets/sets the current state
			envState_t getState();
//			void setState(envState_t);

// Starts the Envelope
			void startEnv();

			// Releases the Envelope
			void releaseEnv();

			// Necessary?
			// Gets and sets the envelope location
//			int getEnvloc();
//			void setEnvloc(int);

			// Sets/gets the envelope properties
			void setAttack(int);
			int getAttack();

			void setDecay(int);
			int getDecay();

			void setSustain(float);
			float getSustain();

			void setRelease(int);
			int getRelease();

		private:
			// Sampling rate of soundcard/ audio process
			int sampFreq;
			int bufferSize;

			// Envelope multiplier buffer
			float * buffer;

			// Current state of envelope
			envState_t state;

			// ADSR - A, D, and R are in ms, S is a multiplier scaled from 0.0 - 1.0
			int attack;
			int decay;
			float sustain;
			int release;

			// Envelope location
			int envloc;

			// The current envelope multiplier
			float envmult;

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
