#pragma once
#include "PolyVoice.hpp"
#include "Mixer.hpp"
#include "IAudio.hpp"
#include "Voice.hpp"
#include "chiputil.hpp"
#include <vector>
#include <iostream>
#include <math.h>

namespace chip
{
	class Module: public IAudio
	{
			public:
				Voice* voice; // Defines the module's current instrument
				Mixer* mixer; // Mixes the module's polyvoices 
 				std::vector<PolyVoice>* polyvoices; //each element represents one note being played
				int next; // The next inactive polyvoice
				
				bool arpeggio;
				int arpsamples; // Indicates the speed of arpeggiation
				int arpcount; // The amount of samples this note has been playing in an arpeggio
				int arpnote; // The current note being played in the arpeggio
				
				bool glissando;
				int glissSamples;
				int glissCount;
				PolyVoice* glissNote;
				float freqSlope;
				
				float glissEnd; // Frequency of the most recent note pressed. Where we are glissing to.
				float glissStart; // Frequency of the second most recent note pressed. Where we are glissing from.
				
				// Volume control
				float volume;
				void setVolume(float);
				
				// Audio buffer
				std::vector<float>* mixedFinal;
				
				Module();
				Module(Voice* voice);
				std::vector<float> advance(int); //create a mixer advance the phase registers of every 
												 //polyvoice in this module (aka - move along the sound wave)
			    
			    void setVoice(int attack, int decay, float sustain, int release, int waveType,
			                  float vibAmp, int vibPeriod, int vibDelay);
			    void activatePolyVoice(int note);
			    void releasePolyVoice(int note);
			    
			    // Sort the notes according to note number
			    void sortPolyVoices();  
			    
			    // Swaps the polyvoices at two locations
			    void swap(int, int);
			    
			    // Moves any polyvoices to be cleaned up to the end of the queue
			    void cleanup();
			    
			    // Shifts all of the elements at and to the right of the index to the right
                void shiftRightAt(int);
                
                // Shifts all of the elements to the right of the index left one
                void shiftLeftAt(int);
			    
			    //Midi Note to Frequency
                float MtoF(int note);
			    
				void printPolyVoices();
	};
}
