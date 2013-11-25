#pragma once

#include "chiputil.hpp"
#include <fstream>
#include <iostream>
#include <vector>

namespace chip
{
class MidiFileNameReader
{
    public:
        std::vector<std::string>* midiFileList;
    
        MidiFileNameReader()
        {
        }
        
        void readFile(); // Reads chiputil::MIDI_FILE_NAMES
        
        std::string getFileNameAt(int index)
        {
            return &midiFileList->at(index);
        }
        
        int numFiles() 
        {
            return midiFileList->size();
        }
        
    private:
        
};  
}
