#pragma once
#include <iostream>
#include <vector>
#include "portmidi.h"
#include "Module.hpp"

// MIDI parsing constants 
// MIDI spec: http://www.midi.org/techspecs/midimessages.php
#define CHANNEL_MASK 0x0F
#define NOTE_ON 0x9             // Used when note is pressed or released. When velocity is zero, treat as note off. 
#define NOTE_OFF 0x8            // Not used but keep it in here anyway.
#define CONTROL_CHANGE 0xB      // Used for pushed buttons and varying values.
#define PROGRAM_CHANGE 0xC      // Used for changing data, like the voices

// Control changes
#define TOGGLE_GLISSANDO 0x41   // "Portamento On/Off"
#define TOGGLE_ARPEGGIO  0x70   // "Sound Controller 1 (default: Sound Variation)"
#define CHANNEL_VOLUME 0x07     // "Channel Volume"
#define VOICE_SELECT 0x00       // "Bank Select." Used to select the new voice

// Program changes
// TODO React to change of 5 position switches
// TODO React to changing the currently selected module/channel

// Song box
#define SONG_SELECT 0xF3        // Song select (song #) TODO or "Tune Request"? 0xF6
#define PLAY 0xFA               // "Start" according to MIDI spec
#define PAUSE 0xFB              // "Continue" according to MIDI spec
#define STOP 0xFC               // "Stop"
#define CHANGE_TEMPO 0xF2       // "Song Position Pointer" according to spec TODO or "Timing clock"? oxF8

// Other
#define RESET 0xFF              // "Reset." Reboot the system. Use sparingly.

namespace chip
{
    class MIDIParser
    {
        private:
            std::vector<Module*>* modules;
            
            int errorPortMIDI(PmError err);
            
            void interpretMIDI(PmEvent data);
            
        public:
            MIDIParser();
            
            void readMIDI(PortMidiStream* mstream, PmEvent* msg);
            
            int connectToMIDIStream(int devID);
            
            void addObject(Module* audioObject);
    };
}
