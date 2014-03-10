#include "PortAudioHandler.hpp"
#include "PortMIDIHandler.hpp"
#include "Mixer.hpp"
#include "ChannelFilter.hpp"
#include "MessagePrinter.hpp"
#include <unistd.h>

int is_int(char const* p) {
	char compare[10];
	sprintf(compare, "%d", atoi(p));
	return strcmp(compare, p) == 0;
}

// Main
int main(int argc, char *argv[]) {

	chip::PortMIDIHandler * PMHandler = new chip::PortMIDIHandler();
	chip::PortAudioHandler * PAHandler = new chip::PortAudioHandler();

	int bufferSize = 512;
	int sampleRate = 44100;
	int numOutChannels = 1;
	int numInChannels = 1;
	int numModules = 5;
	int MIDIDevID = 0;
	int AudioOutDevID = 3;
	int AudioInDevID = 1;
	int verbose = 0;
	extern char *optarg;
	int ch;
	//Scans for argument inputs: -p # binds chipophone to MIDI Port number #, -v makes chipophone behave in verbose mode
	while ((ch = getopt(argc, argv, "dvp:b:s:c:m:a")) != EOF) {
		switch (ch) {
			case 'p':
				// MIDI Port argument
				if (is_int(optarg)) {
					MIDIDevID = atoi(optarg);
				} else {
					fprintf(stderr,
							"MIDI Port takes an integer argument. Specify MIDI Device to be used\n");
					exit(2);
				}
				break;
			case 'a':
				// Audio Device argument
				if (is_int(optarg)) {
					AudioOutDevID = atoi(optarg);
				} else {
					fprintf(stderr,
							"Audio Device takes an integer argument. Specify Audio Device to be used\n");
					exit(2);
				}
				break;
			case 'v':
				// Verbose argument
				verbose = 1;
				printf("Executing in verbose mode...\n");
				break;
			case 'd':
				// Devices argument
				PAHandler->printAudioDevices();
				PMHandler->printMIDIDevices();
				exit(0);
				break;
			case 'b':
				// Buffer Size Argument
				if (is_int(optarg)) {
					bufferSize = atoi(optarg);
				} else {
					fprintf(stderr,
							"Buffer Size takes an integer argument. Specify desired buffer size\n");
					exit(2);
				}
				break;
			case 's':
				// Sample Rate argument
				if (is_int(optarg)) {
					sampleRate = atoi(optarg);
				} else {
					fprintf(stderr,
							"Sample Rate takes an integer argument. Specify desired audio Sampling Rate\n");
					exit(2);
				}
				break;
			case 'c':
				// Audio Channels
				if (is_int(optarg)) {
					numOutChannels = atoi(optarg);
				} else {
					fprintf(stderr,
							"Audio Channels takes an integer argument. Specify number of output audio channels\n");
					exit(2);
				}
				break;
			case 'm':
				// Number of Modules
				if (is_int(optarg)) {
					numModules = atoi(optarg);
				} else {
					fprintf(stderr,
							"Modules takes an integer argument. Specify number of MIDI modules\n");
					exit(2);
				}
				break;

		}
	}

	//TODO: Generate wavetables
	//TODO: generate default voices
	//TODO: Assign voices downstream
	//TODO: Fix Voice Configuration Reader

	// The master mixer for the whole synth
	chip::Mixer * masterMixer = new chip::Mixer(bufferSize, sampleRate);

	/*** Set up the PA Handler. This is where the audio callback is ***/
	PAHandler->connectAudioStream(bufferSize, sampleRate, AudioOutDevID, 0, numOutChannels,
			numInChannels, masterMixer);

	/*** Set up the PM handler ***/
	PMHandler->connectMIDIStream(MIDIDevID);

	/*** Make all MIDI Connections ***/

	// Print incoming MIDI Connections
	if (verbose) {
		chip::MessagePrinter * printer = new chip::MessagePrinter();
		PMHandler->addMIDIDevice(printer);
	}

	while (1) {
		// Read MIDI, forward
		PMHandler->readMIDI();
		masterMixer->cleanup();
	}

	PAHandler->disconnectAudioStream();
	PMHandler->disconnectMIDIStream();

}

