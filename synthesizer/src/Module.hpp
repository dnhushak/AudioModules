#pragma once
#include "PolyVoice.hpp"
#include "Mixer.hpp"
#include "IAudio.hpp"
#include <vector>
#include <iostream>

#define NUM_POLYVOICES 16

namespace chip
{
	class Module: public IAudio
	{
			public:
				//Voice voice //TODO - defines the module's current instrument
				Mixer* mixer;
				std::vector<PolyVoice>* polyvoices; //each element represents one note being played
				Module();
				std::vector<float> advance(int); //create a mixer advance the phase registers of every 
												 //polyvoice in this module (aka - move along the sound wave)
			    
			    void activatePolyVoice(int index, int note, float phase, int frequency);
			    void deactivatePolyVoice(int index);
			    
				void printPolyVoices();
	};
}
