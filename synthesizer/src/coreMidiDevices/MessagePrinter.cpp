#include "MessagePrinter.hpp"

namespace chip {
	void MessagePrinter::MessagePrinter(){
		// Cannot add devices to the Printer, nothing to do.
		maxNumMIDIDevices = 0;
	}

	void MessagePrinter::affect(MIDIMessage * message) {
		std::cout << "MIDI message: " << " Message: " << decodeMessageType(message)
				<< " Channel: " << message->channel << " Data 1: " << message->data1
				<< " Data2 = " << message->data2 << "\n";
	}

	char * MessagePrinter::decodeMessageType(MIDIMessage * message){
		switch (message->type) {
			case 0b1000:
				// Note Off
				return "Note Off       ";
				break;
			case 0b1001:
				// Note On
				return "Note On        ";
				break;
			case 0b1010:
				// Aftertouch (Polyphonic)
				return "Aftertouch     ";
				break;
			case 0b1011:
				// CC
				return "Control Change ";
				break;
			case 0b1100:
				// Program Change
				return "Program Change ";
				break;
			case 0b1101:
				// Aftertouch (Monophonic)
				return "Mono Aftertouch";
				break;
			case 0b1110:
				// Pitch Bend
				return "Pitch Bend     ";
				break;
			case 0b1111:
				// System Message
				return "System Message ";
				break;
			default:
				break;
		}
		return "Unrecognized   ";
	}

}
