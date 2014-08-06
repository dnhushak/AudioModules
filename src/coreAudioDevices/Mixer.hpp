#pragma once
#include "AudioDevice.hpp"
#include "ConnectableDevice.hpp"
#include <list>
#include <iterator>
#include <iostream>

namespace modules {
	class Mixer: public AudioDevice, public ConnectableDevice<AudioDevice> {
		public:
			// Constructor
			Mixer();
			// Advance by a given number of samples (in this case summing all in the AudioList
			sample_t * advance(int);

	};
}
