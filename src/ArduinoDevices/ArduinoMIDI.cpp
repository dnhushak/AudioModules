#include "ArduinoMIDI.h"

namespace arduino {
	
	ArduinoMIDI::ArduinoMIDI(HardwareSerial* port) {
		MIDIport = port;
		buffer = 0;
	}

	void ArduinoMIDI::initialize() {
		MIDIport->begin(31250);
		MIDIport->flush();
		MIDIport->setTimeout(100);
	}

	ArduinoMIDI::~ArduinoMIDI() {
		MIDIport->end();
	}

	void ArduinoMIDI::affect(midi::MIDIMessage * message) {
		if (!isEmpty()) {
			deviceIter = begin();
			while (deviceIter != end()) {
				(*deviceIter)->process(message);
				deviceIter++;
			}
		}
	}

	void ArduinoMIDI::readMIDI() {
		// Find the first status byte
		do
			MIDIport->readBytes(&buffer, 1);
		while (buffer >> 7 == 0);

		midi::MIDIMessage * message = (midi::MIDIMessage*) malloc(
				sizeof(midi::MIDIMessage));
		message->statusType = buffer >> 4;
		message->channel = buffer &= 0b00001111;
		message->data1 = 0;
		message->data2 = 0;

		// Check for system messages that do not have a data byte
		if (buffer < 0b11110100) {

			// Read the first data byte
			MIDIport->readBytes(&buffer, 1);
			message->data1 = buffer;

		}

		// Send the message down the pipe
		affect(message);

		// Clear that message from memory
		free(message);
		return;
	}
	
	void ArduinoMIDI::writeMIDI(midi::MIDIMessage* message) {
		// Reconstruct the status byte
		// Type is the MS nibble
		unsigned char statusByte = (message->statusType << 4);
		// Channel is the LS nibble
		statusByte |= message->channel;

		// Filter out Sysex
		if (message->statusType == midi::SYSTEM
				&& (message->channel == midi::SYSEX
						|| message->channel == midi::SYSEXEND)) {
//			Serial.println("Sysex");

		} else {
			// Non sysex data

			// Write the status byte
			MIDIport->write(statusByte);
//			Serial.println(statusByte, BIN);

			// Don't send data 1 for anything other than non system, MTC, song select, or song poitner messages
			if (message->statusType == midi::SYSTEM
					&& message->channel > midi::SONGSELECT) {

			} else {
				// Write data 1
				MIDIport->write(message->data1);
//				Serial.println(message->data1, BIN);

				// Only write data two on a song position message and non program/monophonic touch messages
				//				if ((message->statusType == SYSTEM
				//						&& (message->channel == SONGPOSITION
				//								|| message->channel == SONGSELECT))
				//						|| (message->statusType != PROGRAM
				//								&& message->statusType != MONOTOUCH)) {
				// Write data 2
				MIDIport->write(message->data2);
//				Serial.println(message->data2, BIN);
				//				}
			}
		}
	}
}
