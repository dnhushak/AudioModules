#include "portmidi.h"
#include <stdio.h>

namespace chip {
	
	class PortMIDIHandler {
		public:
			PmError connectMIDIStream(PmDeviceID devID);

			PmError disconnectMIDIStream();

			void static printMIDIDevices();

			PmStream * getStream();

		private:
			PmStream * mstream;

			PmError errorPortMIDI(PmError err);
	};

}
