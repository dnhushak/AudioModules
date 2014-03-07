#pragma once
#include "AudioEffect.hpp"
#include "chiputil.hpp"
#include <vector>
#include <iostream>

namespace chip {
	class Mixer: public AudioEffect {
		public:
			// Constructor
			Mixer(int);
			// Advance by a given number of samples (in this case summing all in the AudioList
			float * advance(int);

		private:
			// Temp buffer
			float * temp;
	};
}
