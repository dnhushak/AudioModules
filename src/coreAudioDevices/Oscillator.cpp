#include "Oscillator.h"

namespace audio {
// Constructor. Sets buffer size and sample rate
	Oscillator::Oscillator() {

		phase = 0;

		// The maximum value of the phase register
		// !! If you change the type of phase register,
		// also change this value according to limits.h
		phaseMax = UINT_MAX;

		// The truncated current phase, used to access a wavetable index
		phaseTruncated = 0;

		// The amount to truncate the phase by every step increase
		phaseTruncateAmt = 0;

		// The amount to increase the phase by every sample
		stepSize = 0;

		// The current frequency of the oscillator
		frequency = 0;

		wavetable = NULL;

		numParameters=3;
	}

	Oscillator * Oscillator::clone(int) {
		// Create new device
		Oscillator * newDevice = new Oscillator();
		// Set all member variables
		newDevice->state = this->state;
		newDevice->setWavetable(this->wavetable);
		newDevice->setBaseFrequency(this->frequency);
		newDevice->phase = this->phase;

		return newDevice;

	}

	void Oscillator::alter(int paramNum, Parameter p) {
		Device::alter(paramNum, p);
		switch (paramNum) {
			case 1:
				// Frequency
				setBaseFrequency(p.getParam().d);
				break;

			case 2:
				// Note
				setBaseFrequencyMIDI(p.getParam().i);
				break;
		}
		// TODO: Find graceful way to MIDI trigger a change of wavetable
	}

	// Returns a buffer of sample values based on oscillation
	sample_t * Oscillator::advance() {
		for (int i = 0; i < bufferSize; i++) {
			// Grab the truncated current phase value
			phaseTruncated = phase >> phaseTruncateAmt;
			// Use said truncated phase value to read from the wavetable
			buffer[i] = wavetable->getSample(phaseTruncated);
			// Advance the phase
			phase += stepSize;
		}
		return buffer;
	}

//	void Oscillator::process(
//			const sample_t *inBuffer,
//			sample_t *outBuffer,
//			int samplesToProcess,
//			int numChannels) {
//
//		// Account for multichannel buffers
//		int totalSamples = samplesToProcess * numChannels;
//
//		// Go through each sample in the buffer
//		for (int i = 0; i < totalSamples; i++) {
//			outBuffer[i] = inBuffer[i];
//		}
//	}

	// Return the oscillator's current frequency
	float Oscillator::getFrequency() {
		return frequency;
	}

	// Set the oscillator's frequency (recalculates phase stepsize)
	void Oscillator::setBaseFrequency(float newFrequency) {
		if (newFrequency == 0) {
			return;
		} else if (newFrequency < 0) {
			frequency = -newFrequency;
		} else {
			frequency = newFrequency;
		}
		calcStepSize(frequency);
	}

	void Oscillator::setBaseFrequencyMIDI(int MIDINote) {
		if (MIDINote >= 0 && MIDINote < 128) {
			setBaseFrequency(MtoF(MIDINote));
		}
	}

	void Oscillator::calcStepSize(float curFrequency) {
		// Step size calculation
		stepSize = (int) (((curFrequency) * (float) phaseMax) / sampleRate);

		/* Step size is an advancement of phase once every sample
		 * So, its units are technically cycles per sample
		 *
		 * Frequency is cycles per second
		 * Sample Rate is samples per second
		 *
		 * So frequency / sample rate is:
		 *
		 * cycles	  second 	cycles
		 * ------  X  ------- = -------
		 * second	  samples	samples
		 *
		 * This cycles per samples is in a fractional form.
		 * A step size to complete one cycle is the maximum value of the phase + 1
		 * (we are utilizing unsigned integer overflow here -
		 * every time the phase overflows, that is one complete wave cycle)
		 *
		 * So to calculate the correct integer step size, we multiply this
		 * cycles/sample (< 1) value by the maximum value of the phase register
		 *
		 */
	}

	// Sets the wavetable (calculates truncation amount for indexing from the phase)
	void Oscillator::setWavetable(Wavetable * newTable) {
		wavetable = newTable;

		// When we grab the index of the wavetable, we want to use the most significant bits
		// of the phase as the index. In order to keep correct in size with the table, we need
		// to use this equation. The phaseTruncateAmt variable is the amount we need to shift
		// phase right by every time we access the wavetable
		phaseTruncateAmt = ((int) 8 * sizeof(phase)
				- (int) log2((double) wavetable->getTableSize()));

		/* A pictorial explanation:
		 * Say we have 8 bit phase: 00000000
		 * Say our table size is 8 bits: 00000000
		 * The index of our table will be log2(tablesize) bits large, in this case 3
		 * So the 3 most significant bits of the phase are what we will use:
		 * 000_00000
		 *
		 * The phase increases every callback by the stepsize calculation, which is also
		 * based on the size of the phase variable.
		 *
		 * So, say that the frequency works out with the sampling rate such that the
		 * phase increases by 32 every sample:
		 * 00000000
		 * 00010000
		 * 00100000
		 * 00110000
		 * 01000000
		 * ...
		 *
		 * Since we're only using the 3 most significant bits, in these five samples, we
		 * will pull from indices 0, 0, 1, 1, and 2, respectively from the wavetable.
		 *
		 * Even though the wavetable might be relatively small, we can still maintain a very
		 * accurate frequency range, as the lower bits of the phase are still accounted for
		 * and not truncated during a step increase, only during a sample grab.
		 *
		 * In this case, the frequency resolution is totally dependent on the size of the phase
		 * register, and not on the size of the table.
		 *
		 */
	}
}
