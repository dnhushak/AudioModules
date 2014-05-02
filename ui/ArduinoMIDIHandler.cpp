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

	void ArduinoMIDIHandler::affect(MIDIMessage * message){
		//do nothing
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

		Serial.println(statusByte, BIN);
		// Filter out Sysex
		if (message->statusType == SYSTEM
				&& (message->channel == SYSEX || message->channel == SYSEXEND)) {

		} else {
			// Non sysex data

			// Write the status byte
			MIDIport->write(statusByte);

			// Don't send data 1 for anything other than non system, MTC, song select, or song poitner messages
			if (message->statusType == SYSTEM
					&& message->channel > SONGSELECT) {

			} else {
				// Write data 1
				MIDIport->write(message->data1);
				Serial.println(message->data1, BIN);

				// Only write data two on a song position message and non program/monophonic touch messages
				if ((message->statusType == SYSTEM
						&& message->channel == SONGPOSITION)
						|| (message->statusType != PROGRAM
								&& message->statusType != MONOTOUCH)) {
					Serial.println("Printing Second Status byte");
					// Write data 2
					MIDIport->write(message->data2);
					Serial.println(message->data2, BIN);
				} else {
					Serial.println("NOT Printing Second Status byte");
				}
			}
		}
	}
}

