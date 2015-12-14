#include "MessagePrinter.h"

namespace midi {
	MessagePrinter::MessagePrinter() {
	}

	void MessagePrinter::affect(MIDIMessage * message) {

		//Make Magenta
		int cyan = "\033[1;36m";
		// Make default color
		int defcol = "\033[0m";
		cout << cyan;
		printf("MIDI Message | Type: ");
		decodeMessageType(message);
		printf(" | Channel: %02i | Data1: %03i | Data2: %03i | Time: %i",
				message->channel, message->data1, message->data2,
				message->time);
		cout << defcol << "\n";
	}

	void MessagePrinter::decodeMessageType(MIDIMessage * message) {
		switch (message->statusType) {
			case NOTEOFF:
				//0b1000
				// Note Off
				cout << "Note Off       ";
				break;
			case NOTEON:
				//0b1001
				// Note On
				cout << "Note On        ";
				break;
			case POLYTOUCH:
				// 0b1010
				// Aftertouch (Polyphonic)
				cout << "Aftertouch     ";
				break;
			case CC:
				// 0b1011
				// CC
				cout << "Control Change ";
				break;
			case PROGRAM:
				//0b1100
				// Program Change
				cout << "Program Change ";
				break;
			case MONOTOUCH:
				// 0b1101
				// Aftertouch (Monophonic)
				cout << "Mono Aftertouch";
				break;
			case PITCHBEND:
				// 0b1110
				// Pitch Bend
				cout << "Pitch Bend     ";
				break;
			case SYSTEM:
				// 0b1111
				// System Message
				switch (message->channel) {
					case SYSEX:
						cout << "SYSEX          ";
						break;
					case MTC:
						cout << "MIDI Time Code ";
						break;
					case SONGPOSITION:
						cout << "Song Position  ";
						break;
					case SONGSELECT:
						cout << "Song Select    ";
						break;
					case TUNEREQUEST:
						cout << "Tune Request   ";
						break;
					case SYSEXEND:
						cout << "SYSEX End      ";
						break;
					case TIMECLOCK:
						cout << "Time Clock     ";
						break;
					case START:
						cout << "Start Song     ";
						break;
					case CONTINUE:
						cout << "Continue Song  ";
						break;
					case STOP:
						cout << "Stop Song      ";
						break;
					case ACTIVESENSE:
						cout << "Active Sense   ";
						break;
					case RESET:
						cout << "Reset          ";
						break;
				}
				break;
			default:
				cout << "Unrecognized   ";
				break;
		}
	}

}
