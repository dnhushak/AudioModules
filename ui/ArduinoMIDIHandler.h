#ifndef ARDUINOMIDIHANDLER_H
#define ARDUINOMIDIHANDLER_H
#include "MIDIDevice.h"
#include <stdio.h>
#include "Arduino.h"
#include <HardwareSerial.h>

namespace synth {

	class ArduinoMIDIHandler: public MIDIDevice {
		public:
			ArduinoMIDIHandler(HardwareSerial * port);

			void begin();

			// Read from the FIFO serial stream
			void readMIDI();

			void affect(MIDIMessage * message);

			// Write to the output stream
			void writeMIDI(MIDIMessage* message);

			virtual ~ArduinoMIDIHandler();

		private:
			HardwareSerial * MIDIport;
	};

}

#endif
