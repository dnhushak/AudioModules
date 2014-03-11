#include "PortAudioHandler.hpp"
#include "PortMIDIHandler.hpp"
#include "Mixer.hpp"
#include "ChannelFilter.hpp"
#include "Wavetable.hpp"
#include "Oscillator.hpp"
#include "MessagePrinter.hpp"
#include <unistd.h>
#include <stdlib.h>

// Main
int main(int argc, char *argv[]) {

	chip::PortMIDIHandler * PMHandler = new chip::PortMIDIHandler();
	chip::PortAudioHandler * PAHandler = new chip::PortAudioHandler();

	int bufferSize = 512;
	int sampleRate = 32000;
	int numOutChannels = 1;
	int numInChannels = 1;
	int numModules = 5;
	int MIDIDevID = 0;
	int AudioOutDevID = 3;
	int AudioInDevID = -1;
	int verbose = 0;
	extern char *optarg;
	int ch;
	//Scans for argument inputs: -p # binds chipophone to MIDI Port number #, -v makes chipophone behave in verbose mode
	while ((ch = getopt(argc, argv, "dvp:b:s:c:m:a:")) != EOF) {
		switch (ch) {
			case 'p':
				// MIDI Port argument
				MIDIDevID = atoi(optarg);
				break;
			case 'a':
				// Audio Device argument
				AudioOutDevID = atoi(optarg);
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
				bufferSize = atoi(optarg);
				break;
			case 's':
				// Sample Rate argument
				sampleRate = atoi(optarg);
				break;
			case 'c':
				// Audio Channels
				numOutChannels = atoi(optarg);
				break;
			case 'm':
				// Number of Modules
				numModules = atoi(optarg);
				break;

		}
	}

	//TODO: Generate wavetables
	//TODO: generate default voices
	//TODO: Assign voices downstream
	//TODO: Fix Voice Configuration Reader

	// The master mixer for the whole synth
	chip::Mixer * masterMixer = new chip::Mixer(bufferSize, sampleRate);

	/*** Make all Audio Connections ***/

	chip::Wavetable * square = new chip::Wavetable(16);
	float samp;
	for (int i = 0; i<16; i++){
		if (i<8){
			samp = -.2;
		}
		else{
			samp = .2;
		}
		square->setSample(i, samp);
	}


	chip::Oscillator * osc = new chip::Oscillator(bufferSize, sampleRate);
	osc->setFrequency(400);
	osc->setWavetable(square);
	masterMixer->addAudioDevice(osc);

	/*** Set up the PA Handler. This is where the audio callback is ***/
	PaError paerr;
	paerr = PAHandler->connectAudioStream(bufferSize, sampleRate, AudioOutDevID,
			AudioInDevID, numOutChannels, numInChannels, masterMixer);
	if (!paerr == paNoError) {
		exit(0);
	}

	/*** Set up the PM handler ***/
	PmError pmerr;
	pmerr = PMHandler->connectMIDIStream(MIDIDevID);
	if (!pmerr == pmNoError) {
		exit(0);
	}





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

