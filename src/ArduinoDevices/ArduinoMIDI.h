#ifndef ARDUINOMIDI_h_
#define ARDUINOMIDI_h_
#include "MIDIDevice.h"
#include <stdio.h>
#include "Arduino.h"
#include <HardwareSerial.h>

namespace arduino {
	
	class ArduinoMIDI: public midi::MIDIDevice,
			public device::Connectable<midi::MIDIDevice> {
		public:

			ArduinoMIDI(HardwareSerial * port);

			void begin();

			// Read from the FIFO serial stream
			void readMIDI();

			void affect(midi::MIDIMessage * message);

			// Write to the output stream
			void writeMIDI(midi::MIDIMessage* message);

			virtual ~ArduinoMIDI();

		private:
			HardwareSerial * MIDIport;
			char buffer;

	};

}

#endif /* ARDUINOMIDI_h_ */
