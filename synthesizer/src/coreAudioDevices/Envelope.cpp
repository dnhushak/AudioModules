#include "Envelope.hpp"
namespace synth {
	Envelope::Envelope() {
		// Restrict to only one audio device;
		setMaxNumDevices(1);

		// Initialize state to INIT
		envState = INIT;
		envmult = 0.0;
		setRelease(100);
		setAttack(100);
		setDecay(100);
		setSustain(.7);
	}

	// Advance the envelope. Returns a buffer holding the envelope multiplier values
	sample_t * Envelope::advance(int numSamples) {

		/**
		 * This is the ADSR "state machine"
		 * Attack goes from current volume to 1
		 * Decay goes from 1 to the Sustain volume
		 * Release goes from the current volume to 0
		 * Below is a graph of a note's volume going through the states of ADSR
		 *     /\
		 *    /  \__________
		 *   /              \
		 *  /                \
		 * /                  \
		 */

		zeroBuffer();
		if (!isEmpty()) {
			// Initialize the buffer to the first audio device
			memcpy(buffer, front()->advance(numSamples),
					sizeof(sample_t) * numSamples);

			for (int i = 0; i < numSamples; i++) {
				switch (envState) {
					default:
						envmult = 0.0;
						break;
					case ATTACK:
						envmult += Aslope;
						// When the evelope location has hit the number of samples, do a state transition
						if (envloc >= AsampCount) {
							envloc = 0;
							envState = DECAY;
						}
						break;

					case DECAY:
						envmult += Dslope;
						// When the evelope location has hit the number of samples, do a state transition
						if (envloc >= DsampCount) {
							envloc = 0;
							envState = SUSTAIN;
						}
						break;

					case SUSTAIN:
						// Sustain won't automatically transition the state. The state will change on note release.
						envmult = sustain;
						break;

					case RELEASE:
						envmult += Rslope;
						// When the evelope location has hit the number of samples, do a state transition
						if (envloc >= RsampCount) {
							state = INACTIVE;
							envState = DONE;
						}
						break;

				}
				// Apply the envelope multiplier (and cast it accordingly)
				buffer[i] *= (sample_t) envmult;
				// Advance the envelope location
				envloc++;
			}
		} else {
			// If no devices connected to envelope
			zeroBuffer();
		}
		return buffer;
	}

// Gets the state of the envelope (generally for cleanup purposes)
	envState_t Envelope::getEnvState() {
		return envState;
	}

// Starts the envelope
	void Envelope::startEnv() {
		state = ACTIVE;
		envState = ATTACK;
		envloc = 0;
		Aslope = ((1.0 - envmult) / AsampCount);
	}

// Releases the envelope
	void Envelope::releaseEnv() {
		//state = ACTIVE;
		envState = RELEASE;
		envloc = 0;
		Rslope = -(envmult / RsampCount);
	}

	/*** Getters and setters ***/
	void Envelope::setAttack(int newAttack) {
		attack = newAttack;
		AsampCount = (attack * sampleRate) / 1000;
	}

	int Envelope::getAttack() {
		return attack;
	}

	void Envelope::setDecay(int newDecay) {
		decay = newDecay;
		DsampCount = (decay * sampleRate) / 1000;
		Dslope = ((sustain - 1.0) / DsampCount);

	}
	int Envelope::getDecay() {
		return decay;
	}

	void Envelope::setSustain(float newSustain) {
		sustain = newSustain;
		// Decay calculation relies on the sustain value, need to set it here
		setDecay(decay);
	}
	float Envelope::getSustain() {
		return sustain;
	}

	void Envelope::setRelease(int newRelease) {
		release = newRelease;
		RsampCount = (release * sampleRate) / 1000;

	}
	int Envelope::getRelease() {
		return release;
	}
}
