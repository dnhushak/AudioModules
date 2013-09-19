#pragma once
#include "PolyVoice.hpp"
#include "Mixer.hpp"
#include "IAudio.hpp"
#include <vector>
#include <iostream>

#define NUM_POLYVOICES 127

namespace chip
{
	class Module: public IAudio
	{
			public:
				//Voice voice //TODO - defines the module's current instrument
				Mixer* mixer;
				std::vector<PolyVoice>* polyvoices; //each element represents one note being played
				int next; // The next inactive polyvoice
				
				Module();
				std::vector<float> advance(int); //create a mixer advance the phase registers of every 
												 //polyvoice in this module (aka - move along the sound wave)
			    
			    void activatePolyVoice(int note);
			    void deactivatePolyVoice(int note);
			    //Midi Note to Frequency
                float MtoF(int note);
			    
				void printPolyVoices();
	};
}
