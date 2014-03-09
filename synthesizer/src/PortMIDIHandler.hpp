#include "portmidi.h"
#include <stdio.h>

namespace chip {
	
	class PortMIDIHandler: MIDIDevice {
		public:
			PmError connectMIDIStream(PmDeviceID devID);

			PmError disconnectMIDIStream();

			// Read from the FIFO serial stream
			void readMIDI();

			// Parses a PmEvent type and returns a MIDIMessage struct
			MIDIMessage * parseMIDI(PmEvent * data);

			// Forwards all MIDI messages in the stream to all in the MIDIDevice List
			static void affect(MIDIMessage * );

			void static printMIDIDevices();

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
