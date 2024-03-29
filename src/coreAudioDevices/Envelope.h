#ifndef ENVELOPE_h_
#define ENVELOPE_h_
#include "AudioDevice.h"
#include "Connectable.h"

namespace audio {
	using namespace device;
	using namespace std;

	enum envState_t{
		INIT, ATTACK, DECAY, SUSTAIN, RELEASE, DONE
	};

	class Envelope: public Connectable<AudioDevice, AudioDevice> {
		public:
			// Constructor
			Envelope();

			virtual Envelope * clone(int);

			void alter(int paramNum, Parameter p);

			// Advance/fill the buffer
			sample_t * advance();

			//			void process(const sample_t *inBuffer, sample_t *outBuffer,
			//					int samplesToProcess, int numChannels);

			// Gets the current state
			int getEnvState();

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
			/**
			 * Calculates the current envelope multiplier and returns it
			 * @return
			 */
			float calcEnvMult();

			// Current state of envelope
			int envState;

			// ADSR - A, D, and R are in ms, S is a multiplier scaled from 0.0 - 1.0
			int attack;
			int decay;
			float sustain;
			int release;

			// Envelope location
			int envloc;

			// The current envelope multiplier
			float envMult;

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

#endif //ENVELOPE_h_
