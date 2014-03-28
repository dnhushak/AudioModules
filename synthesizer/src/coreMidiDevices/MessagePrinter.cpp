#include "MessagePrinter.hpp"

namespace synth {
	MessagePrinter::MessagePrinter() {
		// Cannot add devices to the Printer, nothing to do.
		maxNumMIDIDevices = 0;
	}

	void MessagePrinter::affect(MIDIMessage * message) {

		//Make Magenta
		std::string cyan = "\033[1;36m";
		// Make default color
		std::string defcol = "\033[0m";
		std::cout << cyan;
		printf("MIDI Message | Type: ");
		decodeMessageType(message);
		printf(" | Channel: %02i | Data1: %03i | Data2: %03i | Time: %i",
				message->channel, message->data1, message->data2, message->time);
		std::cout << defcol << "\n";
	}

	void MessagePrinter::decodeMessageType(MIDIMessage * message) {
		switch (message->type) {
			case 0b1000:
				// Note Off
				std::cout << "Note Off       ";
				break;
			case 0b1001:
				// Note On
				std::cout << "Note On        ";
				break;
			case 0b1010:
				// Aftertouch (Polyphonic)
				std::cout << "Aftertouch     ";
				break;
			case 0b1011:
				// CC
				std::cout << "Control Change ";
				break;
			case 0b1100:
				// Program Change
				std::cout << "Program Change ";
				break;
			case 0b1101:
				// Aftertouch (Monophonic)
				std::cout << "Mono Aftertouch";
				break;
			case 0b1110:
				// Pitch Bend
				std::cout << "Pitch Bend     ";
				break;
			case 0b1111:
				// System Message
				std::cout << "System Message ";
				break;
			default:
				std::cout << "Unrecognized   ";
				break;
		}
	}

}
