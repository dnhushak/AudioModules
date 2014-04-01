#include "portmidi.h"
#include "MIDIDevice.hpp"
#include <stdio.h>
#include <unistd.h>

namespace synth {
	
	class PortMIDIHandler: public MIDIDevice {
		public:
			PortMIDIHandler();

			PmError connectMIDIStream(PmDeviceID devID);

			PmError disconnectMIDIStream();

			// Read from the FIFO serial stream
			void readMIDI();

			// Write to the output stream
			void writeMIDI(MIDIMessage* message);

			// Parses a PmEvent type and returns a MIDIMessage struct
			MIDIMessage * parseMIDI(PmEvent *);

			void printMIDIDevices();

			PortMidiStream * getStream();

			virtual ~PortMIDIHandler();

			void StartCallback();
			void StopCallback();
		private:

			static void * Callback(void * args);


			PmError err;

			pthread_t callback_tid;

			// The PortMIDI stream
			PortMidiStream * mstream;

			// PM error handling
			PmError errorPortMIDI(PmError err);

			// The buffer of MIDI messages
			PmEvent event[128];
	};

}
