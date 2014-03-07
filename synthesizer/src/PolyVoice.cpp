#include "PolyVoice.hpp"

using namespace chip;

PolyVoice::PolyVoice(int initBufferSize, int initSampleRate) {

	bufferSize = initBufferSize;
	sampleRate = initSampleRate;
	osc = new Oscillator(bufferSize, sampleRate);
	vib = new Oscillator(bufferSize, sampleRate);
	osc_env = new Envelope();
	vib_env = new Envelope();
	note = 0;
	baseFrequency = 0;
	vibmult = 0;
}

float * chip::PolyVoice::advance(int numSamples) {

	if (state == INACTIVE) {
		zeroBuffer();
		return buffer;
	}

	for (int i = 0; i < bufferSize; i++) {
		// Grab the oscillator sample
		buffer[i] = osc->advance(1);
		// Apply the oscillator envelope
		buffer[i] *= osc_env->advance(1);

		if (vib_en){
			vibmult = vib->advance(1);
			vibmult *= vib_env->advance(1);
			osc->setFrequency(baseFrequency * vibmult);
		}
	}

	// Check whether or not to deactivate the polyVoice
	if (osc_env->getState() == DONE){
		state = INACTIVE;
	}
	return buffer;
}

devState_t PolyVoice::getState() {
	return state;
}

// Enable vibrato
void PolyVoice::enableVibrato(){
	vib_en = true;
}

// Disable vibrato
void PolyVoice::disableVibrato(){
	vib_en = false;
}

// Start the polyvoice
void PolyVoice::startPolyVoice(int newNote){
	state = ACTIVE;
	note = newNote;
	baseFrequency = MtoF(note);
	osc_env->startEnv();
	vib_env->startEnv();

}

// Release the polyvoice (with envelopes, doesn't necessarily deactivate it)
void PolyVoice::releasePolyVoice(){
	osc_env->releaseEnv();
}

void PolyVoice::setVoice(Voice * newVoice){

}
