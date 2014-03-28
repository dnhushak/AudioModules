#include "PortMIDIHandler.hpp"

namespace synth {

	PmError PortMIDIHandler::connectMIDIStream(PmDeviceID devID) {
		PmError err = Pm_Initialize();
		if (err != pmNoError)
			errorPortMIDI(err);

		int count = Pm_CountDevices();
		if (count == 0)
			fprintf(stderr, "No MIDI devices found\n");
		err = Pm_OpenInput(&mstream, devID, NULL, 512L, NULL, NULL);
		StartCallback();
		if (err != pmNoError)
			errorPortMIDI(err);

		printf("Bound to port %d\n", devID);
		return err;
	}

	PmError PortMIDIHandler::disconnectMIDIStream() {
		MIDIstate = 0;
		PmError err = Pm_Close(mstream);
		if (err != pmNoError)
			errorPortMIDI(err);

		Pm_Terminate();

		return err;
	}

	void PortMIDIHandler::readMIDI() {
		while (Pm_Poll(mstream)) {
			// Grab all MIDI events still in the queue
			int cnt = Pm_Read(mstream, event, 32);
			// Interpret each
			for (int i = 0; i < cnt; i++) {
				// Parse the MIDI and pass it to the affect method
				parseMIDI(&event[i], &msg[i]);
				affect(&msg[i]);
			}
		}
		return;
	}

	void PortMIDIHandler::writeMIDI(MIDIMessage* message) {
		//TODO: complete behavior for midi writing
	}

	MIDIMessage * PortMIDIHandler::parseMIDI(PmEvent * data,
			MIDIMessage * message) {

		// Grab status
		int status = Pm_MessageStatus(data->message);

		// Top four bits is message type
		message->type = status >> 4;
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
		pthread_create(&callback_tid, NULL, PortMIDIHandler::Callback, this);
	}

	void * PortMIDIHandler::Callback(void * args) {
		PortMIDIHandler * PMHandler = (PortMIDIHandler *) args;
		while (PMHandler->getMIDIState() == 1) {
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

}
