#pragma once
#include <iostream>
#include <vector>
#include <unistd.h>
#include "portmidi.h"
#include "Module.hpp"
#include "MIDIController.hpp"

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

#define CHANNEL_MASK 0x0F

namespace chip {
	class MIDIProcessor {
			struct MIDIMessage {
					unsigned char type;
					unsigned char channel;
					unsigned char data1;
					unsigned char data2;
					PmTimestamp time;
			};
		public:

			// Constructor
			MIDIProcessor();

			// Read from the FIFO serial stream
			void readMIDI(PmStream * mstream);

			// Add an AudioDevice to edit
			void addObject(AudioDevice* audioObject);

			// Process the MIDI
			static void processMIDI(MIDIMessage);

		private:

			MIDIMessage * parseMIDI(PmEvent *);

			static void printMIDI(int message, int moduleID, int data1,
					int data2);

			int scaleValue(int value, int min, int max);

			PmEvent msg[32];

			std::vector<AudioDevice *> * audioDeviceList;

			void interpretMIDI(PmEvent data);
	};
}
