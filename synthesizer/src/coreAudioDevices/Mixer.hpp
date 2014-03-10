#pragma once
#include "AudioEffect.hpp"
#include <vector>
#include <iostream>

namespace chip {
	class Mixer: public AudioEffect {
		public:
			// Constructor
			Mixer(int, int);
			// Advance by a given number of samples (in this case summing all in the AudioList
			float * advance(int);

			// Returns the number of objects in this mixer
			int getNumAudioDevices();

			// Sets the maximum number of devices
			void setMaxNumAudioDevices(int);

		private:
			// Temp buffer
			float * temp;
	};
}
