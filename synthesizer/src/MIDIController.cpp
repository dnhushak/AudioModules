#include "MIDIController.hpp"

namespace chip {

	void MIDIController::printMIDI(int message, int moduleID, int data1,
			int data2) {
		std::cout << "MIDI message: " << " Message = " << message
				<< " Module = " << moduleID << " Data1 = " << data1
				<< " Data2 = " << data2 << "\n";
		return;
	}

	void MIDIController::interpretMIDI(int message, int channel, int data1,
			int data2, chip::AudioDevice * audioDevice) {
		switch (message) {
			case NOTE_ON:
				break;
			case NOTE_OFF:
				break;
			case CONTROL_CHANGE:
				switch (data1) {
					case GLISSANDO:
						bool value = (data2 >= 64);
						break;
					case GLISSANDO_SPEED:
						break;
					case ARPEGGIO:
						bool value = (data2 >= 64);
						break;
					case ARPEGGIO_SPEED:
						break;
					case CHANNEL_VOLUME:
						break;
				}
				break;
			case PROGRAM_CHANGE:
				break;
		}
		//printMIDI(message, moduleID, data1, data2);

		return;
	}

	int MIDIController::scaleValue(int value, int min, int max) {
		// Value = 0 results in returning the min
		// Value = 127 results in returning the max
		// Value between 0 and 127 returns a value scaled between min and max
		return value * (max - min) / 127 + min;
	}

}
