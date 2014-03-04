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

//Pi, for generation of sine table
#ifndef M_PI
#define M_PI  (3.14159265)
#endif

namespace chip {
	class AudioProcessor {
		public:
			Mixer * masterMixer;
			std::vector<Module*> * modules;


			AudioProcessor(int, int);
			std::vector<float> * advance(int);
			void cleanup();
			~AudioProcessor();
	};

}

