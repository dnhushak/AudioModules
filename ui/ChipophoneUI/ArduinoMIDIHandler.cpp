#include "ArduinoMIDIHandler.h"

namespace synth {

	ArduinoMIDIHandler::ArduinoMIDIHandler(HardwareSerial* port) {
		MIDIport = port;
	}

	void ArduinoMIDIHandler::begin() {
		MIDIport->begin(31250);
	}

	ArduinoMIDIHandler::~ArduinoMIDIHandler() {
		MIDIport->end();
	}

	void ArduinoMIDIHandler::readMIDI() {
		return;
	}

	void ArduinoMIDIHandler::writeMIDI(MIDIMessage* message) {
		// Reconstruct the status byte
		// Type is the MS nibble
		unsigned char statusByte = (message->statusType << 4);
		// Channel is the LS nibble
		statusByte |= message->channel;

		// Write the status byte
		MIDIport->write(statusByte);

		// Write data 1
		MIDIport->write(message->data1);

		// Write data 2
		MIDIport->write(message->data2);

	}
}

