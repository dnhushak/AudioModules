#pragma once
#include <iostream>
#include <stdio.h>
#include <vector>
#include "portmidi.h"
//#include "MidiFileNameReader.hpp"
//#include "MIDIController.hpp"

#define CHANNEL_MASK 0x0F

// MIDI parsing constants 
// MIDI spec: http://www.midi.org/techspecs/midimessages.php
#define NOTE_ON 0x9             // Used when note is pressed or released. 
                                // When velocity is zero, treat as note off message. 
#define NOTE_OFF 0x8            // Not used but keep it in here anyway.
#define SONG_SELECT 0xF3        // Song select (song #)
#define PLAY 0xFA               // "Start" according to MIDI spec
#define PAUSE 0xFB              // "Continue" according to MIDI spec
#define STOP 0xFC               // "Stop"
#define RECORD 0xF9             // "Undefined. (Reserved)"
#define CHANGE_TEMPO 0xF2       // "Song Position Pointer" according to spec or "Timing clock"? oxF8


namespace chip
{
    class SongboxMidiParser
    {
        public:
            SongboxMidiParser() {}
            
            void readMIDI(PortMidiStream* mstream, PmEvent* msg);
            
            int connectToMIDIStream(int devID);
            
        private:
            int errorPortMIDI(PmError err);

            void printMIDI(int message, int data1, int data2);
            
            void interpretMIDI(PmEvent data);            
    };
}
