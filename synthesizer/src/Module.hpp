#pragma once
#include "PolyVoice.hpp"
#include "Mixer.hpp"
#include <vector>
namespace chip
{
	class Module: public IAudio
	{
			public:
				//Voice voice //TODO - defines the module's current instrument
				//Polyvoice polyvoice //TODO - defines the module's associated polyvoices
				Mixer mixer;
				std::vector<PolyVoice> polyvoices; //each element represents one note being played
				Module();
				std::vector<float> advance(int); //create a mixer advance the phase registers of every 
												 //polyvoice in this module (aka - move along the sound wave)
	};
}
