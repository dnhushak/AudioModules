#pragma once
#include "AudioDevice.hpp"
#include <vector>
#include <iostream>

namespace synth {

	enum envState_t {
		INIT, ATTACK, DECAY, SUSTAIN, RELEASE, DONE,
	};

	class Envelope: public AudioDevice {
		public:
			// Constructor
			Envelope(int, int);

			// Advance/fill the buffer
			float * advance(int);

			// Gets the current state
			envState_t getEnvState();

			// Starts the Envelope
			void startEnv();

			// Releases the Envelope
			void releaseEnv();

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
