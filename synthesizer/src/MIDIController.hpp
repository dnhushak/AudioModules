#pragma once
#include <iostream>
#include "Module.hpp"

// MIDI parsing constants 
// MIDI spec: http://www.midi.org/techspecs/midimessages.php
#define NOTE_ON 0x9             // Used when note is pressed or released. 
                                // When velocity is zero, treat as note off message. 
#define NOTE_OFF 0x8            // Not used but keep it in here anyway.
#define CONTROL_CHANGE 0xB      // Used for pushed buttons and varying values.
#define PROGRAM_CHANGE 0xC      // Used for changing data, like the voices

// Control changes
#define GLISSANDO_TOGGLE 0x41   // "Portamento On/Off" data2 <=63 off, data2 >=64 on 
#define GLISSANDO_SPEED 0x05    // "Portamento Time"
#define ARPEGGIO_TOGGLE  0x50   // "General Purpose Controller 5" data2 <=63 off, data2 >=64 on
#define ARPEGGIO_SPEED 0x51     // "General Purpose Controller 6"
#define CHANNEL_VOLUME 0x07     // "Channel Volume"
#define SELECT_VOICE 0x00       // "Bank Select." Used to select the new voice

// Program changes
// TODO React to change of 5 position switches
// TODO React to changing the currently selected module/channel

// Song box
#define SONG_SELECT 0xF3        // Song select (song #) or "Tune Request"? 0xF6
#define PLAY 0xFA               // "Start" according to MIDI spec
#define PAUSE 0xFB              // "Continue" according to MIDI spec
#define STOP 0xFC               // "Stop"
#define CHANGE_TEMPO 0xF2       // "Song Position Pointer" according to spec or "Timing clock"? oxF8

// Other
#define RESET 0xFF              // "Reset." Reboot the system. Use sparingly.
#define GLISSANDO_MIN  5000
#define GLISSANDO_MAX 50000
#define ARPEGGIO_MIN 500
#define ARPEGGIO_MAX 50000

namespace chip
{
    class MIDIController
    {
        public:
            static void interpretMIDI(int message, int data1, int data2, chip::Module* module);

        private:
            static void noteOn(int note, int velocity, chip::Module* module);
            static void noteOff(int note, chip::Module* module);

            static void glissandoToggle(chip::Module* module);
            static void glissandoSpeed(int scale, chip::Module* module);

            static void arpeggioToggle(chip::Module* module);
            static void arpeggioSpeed(int scale, chip::Module* module);

            static int  scaleValue(int value, int min, int max);

            static void channelVolume(int intensity, chip::Module* module);

            static void selectVoice(int voiceIndex, chip::Module* module);
    };
}
