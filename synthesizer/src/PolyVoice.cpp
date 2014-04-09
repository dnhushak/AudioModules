#include "PolyVoice.hpp"

namespace synth {

	PolyVoice::PolyVoice() {
		note = 0;
		baseFrequency = 0;
		vibmult = 0;
		vib_en = true;
		osc_env.addDevice(&osc);
		vib_ramp.addDevice(&vib);
	}

	PolyVoice::~PolyVoice() {
	}

	sample_t * PolyVoice::advance(int numSamples) {
		zeroBuffer();
		if (state == INACTIVE) {
			return buffer;
		}

		/* We unfortunately have to go through sample by sample, as
		 * the vibrato is also time-based, and needs to alter the frequency of the
		 * oscillator within the callback
		 */
		for (int i = 0; i < numSamples; i++) {
			// Grab the oscillator/envelope sample
			buffer[i] = (osc_env.advance(1))[0];

			if (vib_en) {
				vibmult *= (vib_ramp.advance(1))[0];
				// Add 1 (to prevent 0 and negative frequencies!)
				vibmult += 1;
				osc.setFrequency(baseFrequency * vibmult);
			}
		}
		return buffer;
	}

	// Using the state of the envelope instead of the polyVoice
	devState_t PolyVoice::getState() {
		return osc_env.getState();
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
			osc.setFrequency(baseFrequency);
			vib.setFrequency(5);
			osc_env.startEnv();
			vib_ramp.startRamp();
		}
	}

// Release the polyvoice (with envelopes, doesn't necessarily deactivate it)
	void PolyVoice::releasePolyVoice() {
		osc_env.releaseEnv();
	}

	void PolyVoice::setVoice(Voice * voice) {
		// Oscillator settings
		osc.setWavetable(voice->osc_table);
		osc_env.setAttack(voice->osc_attack);
		osc_env.setDecay(voice->osc_decay);
		osc_env.setSustain(voice->osc_sustain);
		osc_env.setRelease(voice->osc_release);

		// Vibrato settings
		vib.setWavetable(voice->vib_table);
		vib_ramp.setTime(voice->vib_time);
		vib_en = voice->vib_en;

	}

	int PolyVoice::getNote() {
		return note;
	}
}
