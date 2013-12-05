#pragma once
#include <iostream>
#include <vector>
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
            
            void readMIDI(PortMidiStream* mstream, PmEvent* msg);
            
            int connectToMIDIStream(int devID);
            
            void addObject(Module* audioObject);

        private:
            std::vector<Module*>* modules;
            
            int errorPortMIDI(PmError err);
            
            void interpretMIDI(PmEvent data);            
    };
}
