#pragma once

#include "PolyVoice.hpp"
#include "Mixer.hpp"

using namespace chip;
class Module {
        public:
			//Voice voice //defines the modules current instrument TODO LATER
			Mixer mixer;
			std::vector<Polyvoice> polyvoices; //each element represents one note being played
            Module();
			vector<float> advance(int); //create a mixer advance the phase registers of every 
										//polyvoice in this module (aka - move along the sound wave)
};


