#include "PortMIDIHandler.hpp"

namespace synth {

	PortMIDIHandler::PortMIDIHandler() {
		callback_tid = NULL;
		err = Pm_Initialize();
		if (err != pmNoError)
			errorPortMIDI(err);

	}

	PortMIDIHandler::~PortMIDIHandler() {

	}

	void PortMIDIHandler::affect(MIDIMessage* message) {
		writeMIDI(message);
	}

	PmError PortMIDIHandler::openMIDIInput(PmDeviceID devID) {

		int count = Pm_CountDevices();
		if (count == 0)
			fprintf(stderr, "No MIDI devices found!\n");
		err = Pm_OpenInput(&mstream, devID, NULL, 512L, NULL, NULL);

		if (err != pmNoError)
			return errorPortMIDI(err);

		printf("Bound to port %d\n", devID);
		return err;
	}

	PmError PortMIDIHandler::openMIDIOutput(PmDeviceID devID) {

		int count = Pm_CountDevices();
		if (count == 0)
			fprintf(stderr, "No MIDI devices found!\n");
		err = Pm_OpenOutput(&mstream, devID, NULL, 512L, NULL, NULL, 0);

		if (err != pmNoError)
			return errorPortMIDI(err);

		printf("Bound to port %d\n", devID);
		return err;
	}

	PmError PortMIDIHandler::disconnectMIDIStream() {
		state = INACTIVE;
		PmError err = Pm_Close(mstream);
		if (err != pmNoError) {
			return errorPortMIDI(err);
		}

		Pm_Terminate();

		return err;
	}

	void PortMIDIHandler::readMIDI() {
		while (Pm_Poll(mstream)) {
			// Grab all MIDI events still in the queue
			int count = Pm_Read(mstream, event, 32);
			// Interpret each
			for (int i = 0; i < count; i++) {
				// Parse the MIDI and forward it to each attached MIDI Device
				MIDIMessage * message = parseMIDI(&event[i]);
				forward(message);
				free(message);
			}
		}
		return;
	}

	void PortMIDIHandler::writeMIDI(MIDIMessage* inputMessage) {
		//TODO: complete behavior for midi writing
		if (inputMessage->statusType == SYSTEM
				&& (inputMessage->channel == SYSEX || inputMessage->channel == SYSEXEND)) {
			// Do not send sysex messages
			return;
		} else {
			PmEvent msg;
			msg.message = Pm_Message(
					inputMessage->statusType << 4 || inputMessage->channel,
					inputMessage->data1, inputMessage->data2);
			Pm_WriteShort(mstream, 0, msg.message);
		}
	}

	MIDIMessage * PortMIDIHandler::parseMIDI(PmEvent * data) {
		MIDIMessage * message = (MIDIMessage*) malloc(sizeof(MIDIMessage));
		// Grab status
		int status = Pm_MessageStatus(data->message);

		// Top four bits is message type
		message->statusType = status >> 4;
		// Bottom four bits is MIDI Channel
		message->channel = (status & 0x0F);
		// Grab the data bits
		message->data1 = Pm_MessageData1(data->message);
		message->data2 = Pm_MessageData2(data->message);

		// Grab the timestamp
		message->time = data->timestamp;
		return message;
	}

	// Print a list of valid devices
	void PortMIDIHandler::printMIDIDevices() {
		//Make Magenta
		std::string magenta = "\033[1;35m";
		// Make default color
		std::string defcol = "\033[0m";
		std::cout << magenta << "***Valid MIDI Devices: ***\n" << defcol;
		Pm_Initialize();
		int ndev;
		ndev = Pm_CountDevices();
		if (ndev == 0) {
			printf("No MIDI devices connected!\n");
		} else {
			for (int i = 0; i < ndev; i++) {
				const PmDeviceInfo * info = Pm_GetDeviceInfo((PmDeviceID) i);
				if (info->input) {
					printf("Input Device:           ");
				} else if (info->output) {
					printf("Output Device:          ");
				}
				printf("| %d: %s\n", i, info->name);
			}

			std::cout << magenta << "***Default MIDI Devices: ***\n" << defcol;

			PmDeviceID defaultin = Pm_GetDefaultInputDeviceID();
			PmDeviceID defaultout = Pm_GetDefaultOutputDeviceID();
			const PmDeviceInfo * inputinfo = Pm_GetDeviceInfo(
					(PmDeviceID) defaultin);
			const PmDeviceInfo * outputinfo = Pm_GetDeviceInfo(
					(PmDeviceID) defaultout);
			printf("Default Input Device:   | %d: %s\n", defaultin,
					inputinfo->name);
			printf("Default Output Device:  | %d: %s\n", defaultout,
					outputinfo->name);
		}
	}

	PmStream * PortMIDIHandler::getStream() {
		return mstream;
	}

	void PortMIDIHandler::StartCallback() {
		pthread_create(&callback_tid, NULL, Callback, (void *) this);
	}

	void PortMIDIHandler::StopCallback() {
		state = INACTIVE;
		pthread_join(callback_tid, NULL);
		printf("\nMIDI Callback ended, closing stream...");
	}

	void * PortMIDIHandler::Callback(void * args) {
		PortMIDIHandler * PMHandler = (PortMIDIHandler *) args;
		while (PMHandler->getState()) {
			PMHandler->readMIDI();
			usleep(1000);
		}

		return NULL;
	}

	// PortMIDI Error Check
	PmError PortMIDIHandler::errorPortMIDI(PmError err) {
		Pm_Terminate();
		fprintf(stderr, "An error occured while using the portmidi stream\n");
		fprintf(stderr, "Error number: %d\n", err);
		fprintf(stderr, "Error message: %s\n", Pm_GetErrorText(err));
		printMIDIDevices();
		return err;
	}

	void PortMIDIHandler::forward(MIDIMessage* message) {
		deviceIter = begin();
		while (deviceIter != end()) {
			(*deviceIter)->affect(message);
			deviceIter++;
		}
	}
}
