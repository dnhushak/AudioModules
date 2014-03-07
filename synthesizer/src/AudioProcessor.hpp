#pragma once

#include <stdio.h>
#include <math.h>
#include <iostream>
#include <vector>
#include "portaudio.h"
#include "Mixer.hpp"
#include "Module.hpp"
#include "PolyVoice.hpp"
#include "VoiceConfigReader.hpp"
#include "Wavetables.hpp"

namespace chip {
	class AudioProcessor: public AudioDevice {
		public:

			AudioProcessor(int, int, int);

			float * advance(int);

			void cleanup();

		private:
			Mixer * masterMixer;
			std::vector<Module*> * modules;
	};

}

