#include "AudioProcessor.hpp"

namespace chip 
{
    AudioProcessor::AudioProcessor()
    {
        int i;
        masterMixer = new Mixer();
        
        // Create the 5 modules for the synthesizer and add them to the mixer
        for(i = 0; i < 5; i++)
        {
            IAudio* module = new Module();
            masterMixer->addObjects(module);
        }
    }
    
    AudioProcessor::~AudioProcessor()
    {
       
    }
    
}
