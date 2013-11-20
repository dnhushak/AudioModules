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
				
				int arpeggio; // Indicates if this module is in arpeggiation mode and the speed of arpeggiation.
				
				Module();
				Module(Voice* voice);
				std::vector<float> advance(int); //create a mixer advance the phase registers of every 
												 //polyvoice in this module (aka - move along the sound wave)
			    
			    void setVoice(int attack, int decay, float sustain, int release, int waveType);
			    void activatePolyVoice(int note);
			    void releasePolyVoice(int note);
			    
			    // Sort the notes according to note number
			    void sortPolyVoices();  
			    
			    // Swaps the polyvoices at two locations
			    void swap(int, int);
			    
			    // Moves any polyvoices to be cleaned up to the end of the queue
			    void cleanup();
			    
			    // Shifts all of the elements at and to the right of the index to the right
                void shiftAt(int);
			    
			    //Midi Note to Frequency
                float MtoF(int note);
			    
				void printPolyVoices();
	};
}
