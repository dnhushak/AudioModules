#pragma once
#include "AudioEffect.hpp"
#include <list>
#include <iterator>
#include <iostream>

namespace synth {
	class Mixer: public AudioEffect {
		public:
			// Constructor
			Mixer(int, int);
			// Advance by a given number of samples (in this case summing all in the AudioList
			float * advance(int);

			// Sets the maximum number of devices
			void setMaxNumAudioDevices(int);

	};
}
