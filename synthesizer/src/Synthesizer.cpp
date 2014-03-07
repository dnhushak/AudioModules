#include "AudioProcessor.hpp"
#include "MIDIProcessor.hpp"
#include "PortAudioHandler.hpp"
#include "PortMIDIHandler.hpp"
#include "utils.h"

// Main
int main(int argc, char *argv[]) {
	int bufferSize = BUFFER_SIZE;
	int sampleRate = SAMPLE_RATE;
	int numChannels = NUM_AUDIO_CHANNELS;
	int numModules = NUM_MODULES;
	int MIDIDevID = 0;
	int AudioDevID = 0;
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
					AudioDevID = atoi(optarg);
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
				chip::PortAudioHandler::printAudioDevices();
				chip::PortMIDIHandler::printMIDIDevices();
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
					numChannels = atoi(optarg);
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

	chip::AudioProcessor * audioProcessor = new chip::AudioProcessor(bufferSize,
			sampleRate, numModules);

	chip::MIDIProcessor * midiProcessor = new chip::MIDIProcessor();

	chip::PortAudioHandler * PAHandler = new chip::PortAudioHandler();
	PAHandler->connectAudioStream(bufferSize, sampleRate, AudioDevID, NULL, 1,
			0, audioProcessor);

	chip::PortMIDIHandler * PMHandler = new chip::PortMIDIHandler();
	PMHandler->connectMIDIStream(MIDIDevID);

	while (1) {
		midiProcessor->readMIDI();
		audioProcessor->cleanup();
	}

	PAHandler->disconnectAudioStream();
	PMHandler->disconnectMIDIStream();

}

