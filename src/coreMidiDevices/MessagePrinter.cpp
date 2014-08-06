#include "MessagePrinter.hpp"

namespace synth {
	MessagePrinter::MessagePrinter() {
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
				message->channel, message->data1, message->data2,
				message->time);
		std::cout << defcol << "\n";
	}

	void MessagePrinter::decodeMessageType(MIDIMessage * message) {
		switch (message->statusType) {
			case NOTEOFF:
				//0b1000
				// Note Off
				std::cout << "Note Off       ";
				break;
			case NOTEON:
				//0b1001
				// Note On
				std::cout << "Note On        ";
				break;
			case POLYTOUCH:
				// 0b1010
				// Aftertouch (Polyphonic)
				std::cout << "Aftertouch     ";
				break;
			case CC:
				// 0b1011
				// CC
				std::cout << "Control Change ";
				break;
			case PROGRAM:
				//0b1100
				// Program Change
				std::cout << "Program Change ";
				break;
			case MONOTOUCH:
				// 0b1101
				// Aftertouch (Monophonic)
				std::cout << "Mono Aftertouch";
				break;
			case PITCHBEND:
				// 0b1110
				// Pitch Bend
				std::cout << "Pitch Bend     ";
				break;
			case SYSTEM:
				// 0b1111
				// System Message
				switch (message->channel) {
					case SYSEX:
						std::cout << "SYSEX          ";
						break;
					case MTC:
						std::cout << "MIDI Time Code ";
						break;
					case SONGPOSITION:
						std::cout << "Song Position  ";
						break;
					case SONGSELECT:
						std::cout << "Song Select    ";
						break;
					case TUNEREQUEST:
						std::cout << "Tune Request   ";
						break;
					case SYSEXEND:
						std::cout << "SYSEX End      ";
						break;
					case TIMECLOCK:
						std::cout << "Time Clock     ";
						break;
					case START:
						std::cout << "Start Song     ";
						break;
					case CONTINUE:
						std::cout << "Continue Song  ";
						break;
					case STOP:
						std::cout << "Stop Song      ";
						break;
					case ACTIVESENSE:
						std::cout << "Active Sense   ";
						break;
					case RESET:
						std::cout << "Reset          ";
						break;
				}
				break;
			default:
				std::cout << "Unrecognized   ";
				break;
		}
	}

}
