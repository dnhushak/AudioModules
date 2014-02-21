#pragma once
#include <iostream>
#include "Module.hpp"
#include "VoiceConfigReader.hpp"

// MIDI parsing constants 
// MIDI spec: http://www.midi.org/techspecs/midimessages.php
#define NOTE_ON 0x9             // Used when note is pressed or released. 
                                // When velocity is zero, treat as note off message. 
#define NOTE_OFF 0x8            // Not used but keep it in here anyway.
#define CONTROL_CHANGE 0xB      // Used for pushed buttons and varying values.
#define PROGRAM_CHANGE 0xC      // Used for changing data, like the voices

// Control changes
#define GLISSANDO 0x41          // "Portamento On/Off" data2 <=63 off, data2 >=64 on 
#define GLISSANDO_SPEED 0x05    // "Portamento Time"
#define ARPEGGIO 0x50           // "General Purpose Controller 5" data2 <=63 off, data2 >=64 on
#define ARPEGGIO_SPEED 0x51     // "General Purpose Controller 6"
#define CHANNEL_VOLUME 0x07     // "Channel Volume"
#define SELECT_VOICE 0x00       // "Bank Select." Used to select the new voice

// Other
#define RESET 0xFF              // "Reset." Reboot the system. Use sparingly.
#define GLISSANDO_MIN  5000
#define GLISSANDO_MAX 50000
#define ARPEGGIO_MIN 300
#define ARPEGGIO_MAX 3600

namespace chip
{
    class MIDIController
    {
        public:
            static void interpretMIDI(int message, int data1, int data2, chip::Module* module, int moduleID);

        private:
            static void printMIDI(int message, int moduleID, int data1, int data2);
        
            static void noteOn(int note, int velocity, chip::Module* module);
            static void noteOff(int note, chip::Module* module);

            static void glissandoToggle(bool value, chip::Module* module);
            static void glissandoSpeed(int scale, chip::Module* module);

            static void arpeggioToggle(bool value, chip::Module* module);
            static void arpeggioSpeed(int scale, chip::Module* module);

            static int  scaleValue(int value, int min, int max);

            static void channelVolume(int intensity, chip::Module* module);

            static void selectVoice(int voiceIndex, chip::Module* module);
    };
}
