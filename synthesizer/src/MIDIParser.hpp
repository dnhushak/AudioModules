#pragma once
#include <iostream>
#include <vector>
#include "portmidi.h"
#include "Module.hpp"

namespace chip
{
    class MIDIParser
    {
        private:
            std::vector<Module*>* modules;
            
        public:
            MIDIParser();
            
            void doAction(PmEvent data);
            
            void interpretMIDI(int devID);
            
            /*
             * Reads the MIDI input stream
             */
            void readMIDI(int devID);
            
            void addObject(Module* audioObject);
    };
}
