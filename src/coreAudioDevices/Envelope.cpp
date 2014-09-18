#include "Envelope.hpp"
namespace audio {
	Envelope::Envelope() {
		// Restrict to only one audio device;
		setMaxNumDevices(1);

		// Initialize state to INIT
		envState = INIT;
		envMult = 0.0;
		setRelease(100);
		setAttack(100);
		setDecay(100);
		setSustain(.7);
	}

	// Advance the envelope. Returns a buffer holding the envelope multiplier values
	sample_t * Envelope::advance() {

		/*
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

		if (!isEmpty()) {
			// Initialize the buffer to the first audio device
			copyToBuffer(front()->read(), bufferSize);
			for (int i = 0; i < bufferSize; i++) {
				// Calculate and apply the envelope multiplier (and cast it accordingly)
				buffer[i] *= (sample_t) calcEnvMult();
				// Advance the envelope location
				envloc++;
			}
		} else {
			// If no devices connected to envelope
			zeroBuffer();
			envloc += bufferSize;
		}
		return buffer;
	}

// Gets the state of the envelope (generally for cleanup purposes)
	envState_t Envelope::getEnvState() {
		return envState;
	}

// Starts the envelope
	void Envelope::startEnv() {
		state = device::ACTIVE;
		envState = ATTACK;
		envloc = 0;
		Aslope = ((1.0 - envMult) / AsampCount);
	}

// Releases the envelope
	void Envelope::releaseEnv() {
		//state = ACTIVE;
		envState = RELEASE;
		envloc = 0;
		Rslope = -(envMult / RsampCount);
	}

	/*** Getters and setters ***/
	void Envelope::setAttack(int newAttack) {
		if (newAttack >= 0) {
			attack = newAttack;
			AsampCount = msToSamp(attack, sampleRate);
		}
	}

	int Envelope::getAttack() {
		return attack;
	}

	void Envelope::setDecay(int newDecay) {
		if (newDecay >= 0) {
			decay = newDecay;
			DsampCount = msToSamp(decay, sampleRate);
			Dslope = ((sustain - 1.0) / DsampCount);
		}
	}
	int Envelope::getDecay() {
		return decay;
	}

	float Envelope::calcEnvMult() {
		switch (envState) {
			default:
				envMult = 0.0;
				break;
			case ATTACK:
				envMult += Aslope;
				// When the evelope location has hit the number of samples, do a state transition
				if (envloc >= AsampCount) {
					envloc = 0;
					envState = DECAY;
				}
				break;

			case DECAY:
				envMult += Dslope;
				// When the evelope location has hit the number of samples, do a state transition
				if (envloc >= DsampCount) {
					envloc = 0;
					envState = SUSTAIN;
				}
				break;

			case SUSTAIN:
				// Sustain won't automatically transition the state. The state will change on note release.
				envMult = sustain;
				break;

			case RELEASE:
				envMult += Rslope;
				// When the evelope location has hit the number of samples, do a state transition
				if (envloc >= RsampCount) {
					state = device::INACTIVE;
					envState = DONE;
				}
				break;

		}
		return envMult;
	}

	void Envelope::setSustain(float newSustain) {
		if (newSustain >= 0 && newSustain <= 1.0) {
			sustain = newSustain;
			// Decay calculation relies on the sustain value, need to set it here
			setDecay(decay);
		}
	}
	float Envelope::getSustain() {
		return sustain;
	}

	void Envelope::setRelease(int newRelease) {
		if (newRelease >= 0) {
			release = newRelease;
			RsampCount = msToSamp(release, sampleRate);
		}
	}
	int Envelope::getRelease() {
		return release;
	}
}
