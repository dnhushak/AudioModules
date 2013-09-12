#include "AudioProcessor.hpp"

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
            
		    module->activatePolyVoice(0, 80, 90.0, 2100);
            masterMixer->addObjects((IAudio*)module);
        }
    }
    
    std::vector<float> AudioProcessor::advance(int numSamples)
    {
        std::vector<float> buffer = masterMixer->advance(numSamples);
        return buffer;
    }
    
    AudioProcessor::~AudioProcessor()
    {
    }
    
}
