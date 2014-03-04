#pragma once
#include "Wavetables.hpp"
#include "chiputil.hpp"
#include <vector>
#include <iostream>

namespace chip {
	class PolyVoice: public IAudio {
		public:
			PolyVoice();
			~PolyVoice() {
			}

			int count;

			int note;
			unsigned short phase;
			float frequency;

			// State of the polyvoice (ADSR)
			int state;
			int getState();

			unsigned int stepsize();

			// Sets the state of the polyvoice to release
			void releasePolyVoice();

			// Sets the ADSR parameters for this polyvoice
			void setVoice(int, int, float, int, int, float, int, int);

			// Gets and sets the envelope multiplier
			float getEnvmult();
			void setEnvmult(float);

			// Gets and sets the envelope location
			float getEnvloc();
			void setEnvloc(float);

			void setAttack(int);
			void setDecay(int);
			void setSustain(float);
			void setRelease(int);

			virtual std::vector<float> * advance(int);

		private:
			Wavetables* wavetable;

			// ADSR envelope parameters
			int attack;
			int decay;
			float sustain;
			int release;
			int waveType;

			float vibAmp;
			int vibPeriod;
			int vibDelay;
			int vibCount;
			float vibFreq;

			float envmult;  // Envelope multiplier
			float envloc;   // Envelope location

			int AsampCount; // Length (in samples) of the attack
			float Aslope; // Slope of the attack curve

			int DsampCount; // Length (in samples) of the decay
			float Dslope; // Slope of the decay curve

			int RsampCount; // Length (in samples) of the release
			float Rslope; // Slope of the release curve

			virtual ~PolyVoice();
	};

}
