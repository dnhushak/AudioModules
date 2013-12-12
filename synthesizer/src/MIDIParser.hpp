#pragma once
#include <iostream>
#include <vector>
#include <unistd.h>
#include "portmidi.h"
#include "Module.hpp"
#include "MIDIController.hpp"

#define CHANNEL_MASK 0x0F

namespace chip
{
    class MIDIParser
    {
        public:
            MIDIParser();
            
            void readMIDI();
            
            int connectToMIDIStream(int devID);
            
            int disconnectMIDIStream();
            
            void addObject(Module* audioObject);

        private:
            PortMidiStream *mstream;
            PmEvent msg[32];
        
            std::vector<Module*>* modules;
            
            int errorPortMIDI(PmError err);
            
            void interpretMIDI(PmEvent data);            
    };
}
