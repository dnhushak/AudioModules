#pragma once
#include "PolyVoice.hpp"
#include "Mixer.hpp"
#include "IAudio.hpp"
#include "Voice.hpp"
#include <vector>
#include <iostream>
#include "chiputil.hpp"

namespace chip
{
	class Module: public IAudio
	{
			public:
				Voice* voice; // Defines the module's current instrument
				Mixer* mixer; // Mixes the module's polyvoices 
 				std::vector<PolyVoice>* polyvoices; //each element represents one note being played
				int next; // The next inactive polyvoice
				std::vector<int>* activeNotes; // List of active notes
				
				Module();
				std::vector<float> advance(int); //create a mixer advance the phase registers of every 
												 //polyvoice in this module (aka - move along the sound wave)
			    
			    // Creates a voice with an ADSR envelope
			    void setVoice(int, int, float, int);
			    
			    void activatePolyVoice(int note);
			    void releasePolyVoice(int note);
			    
			    // Moves any polyvoices to be cleaned up to the end of the queue
			    void cleanup();
			    
			    //Midi Note to Frequency
                float MtoF(int note);
			    
				void printPolyVoices();
	};
}
