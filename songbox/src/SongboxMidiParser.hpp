#pragma once
#include <iostream>
#include <vector>
#include "portmidi.h"
#include "MidiFileNameReader.hpp"

// MIDI parsing constants 
// MIDI spec: http://www.midi.org/techspecs/midimessages.php
#define CHANNEL_MASK 0x0F
#define NOTE_ON 0x9             // Used when note is pressed or released. When velocity is zero, treat as note off. 
#define NOTE_OFF 0x8            // Not used but keep it in here anyway.

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
class SongboxMidiParser
{
	private:

		int errorPortMIDI(PmError err);

	public:
		MIDIParser();

		void readMIDI(PortMidiStream* mstream, PmEvent* msg);

		void outputMIDI(int devID);

		int connectToMIDIStream(int devID);
};
}
