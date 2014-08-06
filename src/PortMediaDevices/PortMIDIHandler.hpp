#include "portmidi.h"
#include "MIDIDevice.hpp"
#include "ConnectableDevice.hpp"
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <iostream>

namespace modules {
	
	class PortMIDIHandler: public MIDIDevice, public ConnectableDevice<MIDIDevice> {
		public:
			PortMIDIHandler();

			PmError openMIDIInput(PmDeviceID devID);

			PmError openMIDIOutput(PmDeviceID devID);

			PmError disconnectMIDIStream();

			void affect(MIDIMessage * message);

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
			void forward(MIDIMessage * message);

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
