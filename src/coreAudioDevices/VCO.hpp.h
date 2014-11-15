#ifndef VCO_HPP_
#define VCO_HPP_

#include "Oscillator.hpp"
#include "ConnectableDevice.hpp"

namespace audio {
	
	class VCO: public audio::Oscillator, public device::ConnectableDevice<AudioDevice> {
		public:
			VCO();
			sample_t * advance();

			/**
			 * Sets the sensitivity of the VCO, in semitones. Default is 1.0
			 * The range is set such that an input of 1.0 will change the frequency
			 * by the ratio determined by this input number of semitones.
			 *
			 * Example: call ```setSensitivity(3)``` initially. If the second audio
			 * device (i.e. an envelope and/or oscillator) outputs 1, the frequency of
			 * this oscillator will be 3 semitones higher. If the audio device outputs 2,
			 * the oscillator frequency will be 6 semitones higher. Negative numbers also work
			 * (-1.0 will make it drop by 3 semitones), as well as subtones (decimal values of semitones)
			 *
			 * The variable semitones is just a multiplier, and the ratio is calculated every time the current frequency
			 * is recalculated
			 * @param semitones Number of semitones an audio input of 1.0 will swing the oscillator frequency by
			 */
			void setSensitivity(float semitones);

			/**
			 * Adjusts the current frequency based on an input adjustment float.
			 * This float is multiplied by the sensitivity for scaling purposes, giving a frequency ratio.
			 * This frequency ratio is then multiplied by the base frequency to get the frequency adjustment in hertz.
			 * This frequency adjustment is then added (or subtracted, if negative) to the base frequency.
			 * @param frequencyAdjust The input value (generally a float between 0 and 1) to adjust the frequency by
			 * (is also affected by the sensitivity, set in ```setSensitivity()```
			 */
			void adjustFrequency(float frequencyAdjust);

			virtual ~VCO();
		private:
			float sensitivity;
	};

}

#endif /* VCO_HPP_ */
