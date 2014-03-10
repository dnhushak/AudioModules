#include "MessagePrinter.hpp"

namespace chip {
	MessagePrinter::MessagePrinter() {
		// Cannot add devices to the Printer, nothing to do.
		maxNumMIDIDevices = 0;
	}

	void MessagePrinter::affect(MIDIMessage * message) {
		std::cout << "MIDI message: " << " Message: ";
		decodeMessageType(message);
		std::cout << " Channel: " << message->channel << " Data 1: "
				<< message->data1 << " Data2 = " << message->data2 << "\n";
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
