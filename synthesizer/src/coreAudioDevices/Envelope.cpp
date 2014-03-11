#include "Envelope.hpp"

chip::Envelope::Envelope(int initBufferSize, int initSampleRate) {
	resizeBuffer(initBufferSize);
	changeSampleRate(initSampleRate);

	// Initialize state to INIT
	state = INIT;

	setRelease(100);
	setAttack(100);
	setDecay(100);
	setSustain(.7);
}

// Advance the envelope. Returns a buffer holding the envelope multiplier values
float * chip::Envelope::advance(int numSamples) {

	// This is the ADSR "state machine"
	// Attack goes from 0 volume to 1
	// Decay goes from 1 to the Sustain volume
	// Release goes from the Sustain volume to 0
	// Below is a graph of a notes volume going through the states of ADSR
	//
	//     /\
	//    /  \__________
	//   /              \
	//  /                \
	// /                  \
	//
	// | A |D|    S    | R |

	for (int i = 0; i < numSamples; i++) {
		switch (state) {
			default:
				envmult = 0.0;
				break;
			case ATTACK:
				envmult += Aslope;
				// When the evelope location has hit the number of samples, do a state transition
				if (envloc >= AsampCount) {
					envloc = 0;
					state = DECAY;
				}
				break;

			case DECAY:
				envmult += Dslope;
				// When the evelope location has hit the number of samples, do a state transition
				if (envloc >= DsampCount) {
					envloc = 0;
					state = SUSTAIN;
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
					state = DONE;
				}
				break;

		}
		buffer[i] = envmult;
		envloc++;
	}
	return buffer;
}

// Gets the state of the envelope (generally for cleanup purposes)
chip::envState_t chip::Envelope::getState() {
	return state;
}

// Starts the envelope
void chip::Envelope::startEnv() {
	state = ATTACK;
	envmult = 0.0;
	envloc = 0;
	Aslope = ((1.0 - envmult) / AsampCount);
}

// Releases the envelope
void chip::Envelope::releaseEnv() {
	state = RELEASE;
	envloc = 0;
	Rslope = -(envmult / RsampCount);
}

/*** Getters and setters ***/
void chip::Envelope::setAttack(int newAttack) {
	attack = newAttack;
	AsampCount = (attack * sampleRate) / 1000;
}

int chip::Envelope::getAttack() {
	return attack;
}

void chip::Envelope::setDecay(int newDecay) {
	decay = newDecay;
	DsampCount = (decay * sampleRate) / 1000;
	Dslope = ((sustain - 1.0) / DsampCount);

}
int chip::Envelope::getDecay() {
	return decay;
}

void chip::Envelope::setSustain(float newSustain) {
	sustain = newSustain;
	// Decay calculation relies on the sustain value, need to set it here
	setDecay(decay);
}
float chip::Envelope::getSustain() {
	return sustain;
}

void chip::Envelope::setRelease(int newRelease) {
	release = newRelease;
	RsampCount = (release * sampleRate) / 1000;

}
int chip::Envelope::getRelease() {
	return release;
}
