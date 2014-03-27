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

			// Write to the output stream
			void writeMIDI(MIDIMessage* message);

			// Parses a PmEvent type and returns a MIDIMessage struct
			MIDIMessage * parseMIDI(PmEvent *, MIDIMessage * );

			void printMIDIDevices();

			PortMidiStream * getStream();

		private:
			// The PortMIDI stream
			PortMidiStream * mstream;

			// PM error handling
			PmError errorPortMIDI(PmError err);

			// The buffer of MIDI messages
			PmEvent event[32];

			MIDIMessage msg[32];
	};

}
