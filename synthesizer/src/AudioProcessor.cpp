#include "AudioProcessor.hpp"

namespace chip 
{
    AudioProcessor::AudioProcessor()
    {
        int i;
        masterMixer = new Mixer();
        modules = new std::vector<Module*>();
        
        std::cout << "Creating audio processor!\n";
        
        // Create the first and only instance of the Wavetables.
        chip::Wavetables* wavetable = chip::Wavetables::getInstance();
        
        // Create the 5 modules for the synthesizer and add them to the mixer
        for(i = 0; i < 5; i++)
        {
            Module* module = new Module();
            
            //module->activatePolyVoice(40);
                        
            modules->push_back(module);
            masterMixer->addObjects((IAudio*)module);
        }
        
        //(*modules)[0]->activatePolyVoice(40);
        //(*modules)[0]->activatePolyVoice(44);
        //(*modules)[0]->activatePolyVoice(47);
                
        //std::vector<float> buffer = masterMixer->advance(FRAMES_PER_BUFFER);
        
        //std::cout << "Advancing master mixer: " << buffer[0] << "\n";
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
