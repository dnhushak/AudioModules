#include "VCO.hpp"

namespace audio {
	
	VCO::VCO() {
		setBaseFrequency(100);
		setSensitivity(1);
	}
	
	sample_t * VCO::advance() {
		// If there is a connected device...
		if (!isEmpty()) {
			// Copy its buffer to the VCO's buffer
			copyToBuffer(front()->read(), bufferSize);
		} else {
			// Otherwise, zero the VCO's buffer
			zeroBuffer();
		}
		for (int i = 0; i < bufferSize; i++) {
			// The frequency adjustments are currently held in each buffer position
			// They will either be the connected objects buffer, if such an object exists,
			// Else they will all be zero.

			// Use these values currently in the buffer to adjust the frequency
			adjustFrequency((float)buffer[i]/(float)sampleMax);

			// Grab the truncated current phase value
			phaseTruncated = phase >> phaseTruncateAmt;

			// Use said truncated phase value to read from the wavetable
			buffer[i] = wavetable->getSample(phaseTruncated);

			// Advance the phase
			phase += stepSize;
		}
		return buffer;
	}

	void VCO::setSensitivity(float semitones) {
		sensitivity = semitones;
	}

	void VCO::adjustFrequency(float frequencyAdjust) {
		/*
		 * Calculate the ratio: based on the input adjustment x the sensitivity, which is in semitones
		 * This is used to calculate the frequency ratio, later used to calculate the step size
		 */
		freqRatio = semitonesToRatio(frequencyAdjust * sensitivity);
		calcStepSize(frequency * freqRatio);
	}

	VCO::~VCO() {
	}

}
