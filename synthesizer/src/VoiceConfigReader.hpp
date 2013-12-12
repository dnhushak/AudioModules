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
        static VoiceConfigReader* getInstance() 
        {
            if(!instanceFlag)
            {
                single = new VoiceConfigReader();
                return single;
            }
            else
            {
                return single;
            }
        }
        
        ~VoiceConfigReader()
        {
            instanceFlag = false;
        }
        
        Voice* getVoiceAt(int index)
        {
            return &voices->at(index);
        }
        
        int numVoices() 
        {
            return voices->size();
        }
        
    private:
        static bool instanceFlag;
        static VoiceConfigReader *single;
        std::vector<Voice>* voices;
        
        int convertWaveType(std::string waveTypeString);
        
        void readFile(); // Reads chiputil::VOICE_CONFIG_FILE
        
        VoiceConfigReader()
        {
            //private constructor
            instanceFlag = true;
            readFile();
        }
};  
}
