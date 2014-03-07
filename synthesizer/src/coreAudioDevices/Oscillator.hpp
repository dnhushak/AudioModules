#pragma once
#include "AudioEffect.hpp"
#include "chiputil.hpp"
#include <vector>
#include <iostream>

namespace chip {
	class Oscillator: public AudioDevice {
		public:
			// Constructor
			Oscillator(int, int);

			// Advance by a given number of samples (in this case summing all in the AudioList
			float * advance(int);

			// Based on the frequency and sample rate, determine how much to advance the phase register
			unsigned int stepsize();

		private:
			unsigned short phase;
			float frequency;

			Wavetables * wavetable;
	};
}
