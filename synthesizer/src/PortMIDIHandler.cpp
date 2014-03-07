#include "PortMIDIHandler.hpp"

namespace chip {

	PmError PortMIDIHandler::connectMIDIStream(PmDeviceID devID) {
		PmError err = Pm_Initialize();
		if (err != pmNoError)
			errorPortMIDI(err);

		int count = Pm_CountDevices();
		if (count == 0)
			fprintf(stderr, "No MIDI devices found\n");

		err = Pm_OpenInput(&mstream, devID, NULL, 512L, NULL, NULL);
		if (err != pmNoError)
			errorPortMIDI(err);

		printf("Bound to port %d\n", devID);
		return err;
	}

	PmError PortMIDIHandler::disconnectMIDIStream() {
		PmError err = Pm_Close(mstream);
		if (err != pmNoError)
			errorPortMIDI(err);

		Pm_Terminate();

		return err;
	}

	// Print a list of valid devices
	void static PortMIDIHandler::printMIDIDevices() {
		printf("***Valid MIDI Devices: ***");
		Pm_Initialize();
		int ndev;
		ndev = Pm_CountDevices();
		for (int i = 0; i < ndev; i++) {
			const PmDeviceInfo * info = Pm_GetDeviceInfo((PmDeviceID) i);
			if (info->input) {
				printf("Input Device:          ");
			} else if (info->output) {
				printf("Output Device:           ");

			}
			printf("%d: %s\n", i, info->name);
		}

		PmDeviceID defaultin = Pm_GetDefaultInputDeviceID();
		PmDeviceID defaultout = Pm_GetDefaultOutputDeviceID()();
		const PmDeviceInfo * inputinfo = Pm_GetDeviceInfo(
				(PmDeviceID) defaultin);
		const PmDeviceInfo * outputinfo = Pm_GetDeviceInfo(
				(PmDeviceID) defaultout);
		printf("Default Input Device:   %d: %s\n", defaultin, inputinfo->name);
		printf("Default Output Device:  %d: %s\n", defaultout,
				outputinfo->name);

	}

	PmStream * PortMIDIHandler::getStream(){
		return mstream;
	}

	// PortMIDI Error Check
	PmError PortMIDIHandler::errorPortMIDI(PmError err) {
		Pm_Terminate();
		fprintf(stderr, "An error occured while using the portmidi stream\n");
		fprintf(stderr, "Error number: %d\n", err);
		fprintf(stderr, "Error message: %s\n", Pa_GetErrorText(err));
		printMIDIDevices();
		return err;
	}

}
