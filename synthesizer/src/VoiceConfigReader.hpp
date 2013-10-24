#pragma once

#include "chiputil.hpp"
#include "Voice.hpp"
#include <fstream>
#include <iostream>
#include <vector>
namespace chip
{
class VoiceConfigReader
{
    public:
        std::vector<Voice>* voices;
    
        VoiceConfigReader()
        {
        }
        
        void readFile(); // Reads chiputil::VOICE_CONFIG_FILE
        
        Voice* getVoiceAt(int index)
        {
            return &voices->at(index);
        }
        
        int numVoices() 
        {
            return voices->size();
        }
        
    private:
        int convertWaveType(std::string waveTypeString);
};  
}
