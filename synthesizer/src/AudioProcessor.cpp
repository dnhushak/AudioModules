#include "AudioProcessor.hpp"

#include "Wavetables.hpp"

namespace chip 
{
    AudioProcessor::AudioProcessor()
    {
        int i;
        masterMixer = new Mixer();
        
        std::cout << "Creating audio processor!\n";
        
        // Create the first and only instance of the Wavetables.
        chip::Wavetables* wavetable = chip::Wavetables::getInstance();
        
        // Create the 5 modules for the synthesizer and add them to the mixer
        for(i = 0; i < 5; i++)
        {
            Module* module = new Module();
            
            PolyVoice* polyvoice1 = new PolyVoice();
            polyvoice1->frequency = 210;
            polyvoice1->note = 80;
		    polyvoice1->isActive = 1;
		    
		    PolyVoice* polyvoice2 = new PolyVoice();
            polyvoice2->frequency = 1000;
            polyvoice2->note = 80;
		    polyvoice2->isActive = 0;
		    
		    PolyVoice* polyvoice3 = new PolyVoice();
            polyvoice3->frequency = 1000;
            polyvoice3->note = 80;
		    polyvoice3->isActive = 0;
		    
		    std::vector<PolyVoice>* pointer = module->polyvoices; 
		    
            (*pointer)[0] = *polyvoice1;
            //module->polyvoices->push_back(*polyvoice2);
            //module->polyvoices->push_back(*polyvoice3);
            
            masterMixer->addObjects((IAudio*)module);
        }
    }
    
    AudioProcessor::~AudioProcessor()
    {
    }
    
}
