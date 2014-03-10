#include "portmidi.h"
#include "MIDIDevice.hpp"
#include <stdio.h>

namespace chip {
	
	class PortMIDIHandler: public MIDIDevice {
		public:
			PmError connectMIDIStream(PmDeviceID devID);

			PmError disconnectMIDIStream();

			// Read from the FIFO serial stream
			void readMIDI();

			// Parses a PmEvent type and returns a MIDIMessage struct
			MIDIMessage * parseMIDI(PmEvent * data);

			// Forwards all MIDI messages in the stream to all in the MIDIDevice List
			//void affect(MIDIMessage *);

			void printMIDIDevices();

			PmStream * getStream();

		private:
			// The PortMIDI stream
			PmStream * mstream;

			// PM error handling
			PmError errorPortMIDI(PmError err);

			// The buffer of MIDI messages
			PmEvent msg[32];
	};

}
