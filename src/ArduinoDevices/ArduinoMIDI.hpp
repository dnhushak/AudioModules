#ifndef ARDUINOMIDI_HPP_
#define ARDUINOMIDI_HPP_
#include "MIDIDevice.hpp"
#include <stdio.h>
#include "Arduino.h"
#include <HardwareSerial.h>

namespace arduino {
	
	class ArduinoMIDI: public midi::MIDIDevice, public device::ConnectableDevice<midi::MIDIDevice> {
		public:

			ArduinoMIDI(HardwareSerial * port);

			void begin();

			// Read from the FIFO serial stream
			void readMIDI();

			void affect(midi::MIDIMessage * message);

			// Write to the output stream
			void writeMIDI(midi::MIDIMessage* message);


		private:
			HardwareSerial * MIDIport;
			virtual ~ArduinoMIDI();
	};

}

#endif /* ARDUINOMIDI_HPP_ */
