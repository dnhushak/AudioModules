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
        
        // Read from VoiceConfig file.
	    VoiceConfigReader* voiceReader = new VoiceConfigReader();
	    voiceReader->readFile();
	    bool validFile = voiceReader->numVoices() >= 5;
	    if (!validFile)
	    {
	        std::cout << "Error reading voice config file. Not enough voices.\n";
	    }
        
        // Create the 5 modules for the synthesizer and add them to the mixer
        for(i = 0; i < 5; i++)
        {
            Module* module = new Module();
            if (validFile)
            {
                module = new Module(voiceReader->getVoiceAt(i));
            }
            
            modules->push_back(module);
            masterMixer->addObjects((IAudio*)module);
        }
                
        std::cout << "** TESTING **\n";
        for (i=0; i<modules->size(); i++)
        {
            std::cout << (*modules)[i]->voice->getWaveType() << "\n";
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
