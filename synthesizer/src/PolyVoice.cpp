#include "PolyVoice.hpp"

using namespace chip;

PolyVoice::PolyVoice(int initBufferSize, int initSampleRate) {
	resizeBuffer(initBufferSize);
	changeSampleRate(initSampleRate);
	osc = new Oscillator(bufferSize, sampleRate);
	vib = new Oscillator(bufferSize, sampleRate);
	osc_env = new Envelope(bufferSize, sampleRate);
	vib_ramp = new Ramp(bufferSize, sampleRate);
	note = 0;
	baseFrequency = 0;
	vibmult = 0;
	vib_en = true;
	state = ACTIVE;
}

chip::PolyVoice::~PolyVoice() {
	delete osc;
	delete vib;
	delete osc_env;
	delete vib_ramp;
}

float * chip::PolyVoice::advance(int numSamples) {
	if (state == INACTIVE) {
		zeroBuffer();
		return buffer;
	}

	/* We unfortunately have to go through sample by sample, as
	 * the vibrato is also time-based, and needs to alter the frequency of the
	 * oscillator within the callback
	 */
	for (int i = 0; i < bufferSize; i++) {
		// Grab the oscillator sample
		buffer[i] = (osc->advance(1))[0];
		// Apply the oscillator envelope
		buffer[i] *= (osc_env->advance(1))[0];

		if (vib_en) {
			// Grab the sine oscillator
			vibmult = (vib->advance(1))[0];
			// Grab the envelope for vibrato
			vibmult *= (vib_ramp->advance(1))[0];
			// Scale the vibrato
			vibmult *= .01;
			// Add 1 (to prevent 0 and negative frequencies!)
			vibmult += 1;
			osc->setFrequency(baseFrequency * vibmult);
		}
	}

	// Check whether or not to deactivate the polyVoice
	if (osc_env->getEnvState() == DONE) {
		state = INACTIVE;
	}
	return buffer;
}

devState_t PolyVoice::getState() {
	return state;
}

// Enable vibrato
void PolyVoice::enableVibrato() {
	vib_en = true;
}

// Disable vibrato
void PolyVoice::disableVibrato() {
	vib_en = false;
}

// Start the polyvoice
void PolyVoice::startPolyVoice(int newNote) {
	if (newNote > 0 && newNote <= 127) {
		state = ACTIVE;
		note = newNote;
		baseFrequency = MtoF(note);
		osc->setFrequency(baseFrequency);
		vib->setFrequency(5);
		osc_env->startEnv();
		vib_ramp->startRamp();
	}
}

// Release the polyvoice (with envelopes, doesn't necessarily deactivate it)
void PolyVoice::releasePolyVoice() {
	osc_env->releaseEnv();
}

void PolyVoice::setVoice(Voice * voice) {
	// Oscillator settings
	osc->setWavetable(voice->osc_table);
	osc_env->setAttack(voice->osc_attack);
	osc_env->setDecay(voice->osc_decay);
	osc_env->setSustain(voice->osc_sustain);
	osc_env->setRelease(voice->osc_release);

	// Vibrato settings
	vib->setWavetable(voice->vib_table);
	vib_ramp->setTime(voice->vib_time);
	vib_en = voice->vib_en;

}

int PolyVoice::getNote() {
	return note;
}
