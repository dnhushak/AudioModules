#include "MIDIProcessor.hpp"

namespace chip {

	void MIDIProcessor::MIDIProcessor() {

	}

	/* Refer to MIDI spec
	 * http://www.midi.org/techspecs/midimessages.php
	 */
	chip::MIDIProcessor::MIDIMessage * MIDIProcessor::parseMIDI(
			PmEvent * data) {

		// MIDIMessage struct to be returned
		MIDIMessage * message = new MIDIMessage;

		// Grab status
		int status = Pm_MessageStatus(data->message);

		// Top four bits is message type
		message->type = status >> 4;
		// Bottom four bits is MIDI Channel
		int channel = (status & 0x0F);

		// Grab the data bits
		message->data1 = Pm_MessageData1(data->message);
		message->data2 = Pm_MessageData2(data->message);

		// Grab the timestamp
		message->time = data->timestamp;
		return message;
	}

	void MIDIProcessor::readMIDI(PortMidiStream * mstream) {
		while (Pm_Poll(mstream)) {
			int cnt = Pm_Read(mstream, msg, 32);
			for (int i = 0; i < cnt; i++) {
				interpretMIDI(msg[i]);
			}
		}
		return;
	}

}
