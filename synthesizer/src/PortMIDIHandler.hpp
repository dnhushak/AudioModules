#include "portmidi.h"
#include <stdio.h>

namespace chip {
	
	class PortMIDIHandler: MIDIDevice {
		public:
			PmError connectMIDIStream(PmDeviceID devID);

			PmError disconnectMIDIStream();

			void static printMIDIDevices();

			PmStream * getStream();

			// Read from the FIFO serial stream
			void readMIDI(PmStream * mstream);

			// Process the MIDI
			static void processMIDI(MIDIMessage);

		private:
			PmStream * mstream;

			PmError errorPortMIDI(PmError err);

			PmEvent msg[32];

			void interpretMIDI(PmEvent data);
	};

}
