#include "PolyVoice.hpp"

using namespace chip;

PolyVoice::PolyVoice(int initSize) {

	bufferSize = initSize;
	buffer = new float[bufferSize];

	// These values mirror those of the Voice type values, but are also included here,
	// should any multitimbral sort of syntheseis be needed (future-proofing)

	// Initialize everything to 0
	note = 0;
	phase = 0;
	frequency = 0.0;
	vibFreq = 0.0; // The vibrato frequency to add to the wave
	waveType = SQUARE;
	envelope = true;

	waveType = SQUARE;

	vibAmp = 0;
	vibPeriod = 0;
	vibDelay = 0;
	vibCount = 0;
	vibFreq = 0.0;

	wavetable = Wavetables::getInstance();
}

float * chip::PolyVoice::advance(int numSamples) {

	if (state == OFF || state == ON) {
		zeroBuffer();
		return buffer;
	}

	int phase_truncated = 16 - POWER;
	for (int i = 0; i < bufferSize; i++) {
		buffer[i] = wavetable->getSample(waveType,
				((int) phase) >> (phase_truncated));

		phase += stepsize();
		if (envelope) {
			advanceEnvelope();
			buffer[i] *= envmult;
		}
	}

	return buffer;
}


int PolyVoice::getState() {
	return state;
}

void PolyVoice::releasePolyVoice() {
	state = RELEASE;
	envloc = 0;
	Rslope = -(envmult / RsampCount);
}

unsigned int PolyVoice::stepsize() {
//Maximum value of phase scale (16^4 in this case)
	int step;

//Our equation!
	step = ((frequency + vibFreq) * PHASESCALE) / SAMPLE_RATE;
	return step;
}

void PolyVoice::setVoice(int attack, int decay, float sustain, int release,
		int waveType, float vibAmp, int vibPeriod, int vibDelay) {

	this->waveType = waveType;
	this->vibAmp = vibAmp;
	this->vibPeriod = vibPeriod;
	this->vibDelay = vibDelay;

	envmult = 0;
	envloc = 0;

	vibCount = 0;
	vibFreq = 0.0;
}

/*** Getters and setters ***/

float PolyVoice::getEnvmult() {
	return envmult;
}

void PolyVoice::setEnvmult(float newmult) {
	envmult = newmult;
}

int PolyVoice::getEnvloc() {
	return envloc;
}

void PolyVoice::setEnvloc(int newloc) {
	envloc = newloc;
}

void PolyVoice::setAttack(int newAttack) {
	attack = newAttack;
	AsampCount = (attack * SAMPLE_RATE) / 1000;
	Aslope = 1.0 / AsampCount;

// Calculation explanation:
// Attack - ms, SAMPLE_RATE - sample/s, 1000 ms/s

// msec * sample   sec
// -----  ------ ---------  = # of samples
//         sec    1k msec

}

void PolyVoice::setDecay(int newDecay) {
	decay = newDecay;
	DsampCount = (decay * SAMPLE_RATE) / 1000;
	Dslope = (sustain - 1.0) / DsampCount;
}

void PolyVoice::setSustain(float newSustain) {
	sustain = newSustain;
}

void PolyVoice::setRelease(int newRelease) {
	release = newRelease;
	RsampCount = (release * SAMPLE_RATE) / 1000;
}

void PolyVoice::zeroBuffer() {
	for (int i = 0; i < bufferSize; i++) {
		buffer[i] = 0.0;
	}
}
