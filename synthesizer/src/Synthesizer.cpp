#include "PortAudioHandler.hpp"
#include "PortMIDIHandler.hpp"
#include "Mixer.hpp"
#include "ChannelFilter.hpp"
#include "Wavetable.hpp"
#include "Oscillator.hpp"
#include "MessagePrinter.hpp"
#include "Module.hpp"
#include "Voice.hpp"
#include <math.h>
#include <unistd.h>
#include <stdlib.h>

std::vector<chip::Wavetable *> * GenerateChipTables() {
	std::vector<chip::Wavetable *> * tables =
			new std::vector<chip::Wavetable *>(0);
	chip::Wavetable* sawtooth = new chip::Wavetable(16);
	chip::Wavetable* triangle = new chip::Wavetable(16);
	chip::Wavetable* square50 = new chip::Wavetable(16);
	chip::Wavetable* square25 = new chip::Wavetable(16);
	chip::Wavetable* noise = new chip::Wavetable(256);
	chip::Wavetable* vibrato = new chip::Wavetable(256);

	tables->push_back(sawtooth);
	tables->push_back(triangle);
	tables->push_back(square50);
	tables->push_back(square25);
	tables->push_back(noise);
	tables->push_back(vibrato);

	//Divide 16 into four regions
	int quarter = 4;
	int half = 2 * quarter;
	int three_fourths = 3 * quarter;

	//Cycle through the entirety of 16 and generate triangle, and square waves
	//The triangle wave statement takes the current index of the for loop, casts it to a float (to do division), and scales it to do the correct math in triangle wave generation
	for (int i = 0; i < 16; i++) {
		sawtooth->setSample(i, (((float) i / 8) - 1));

		//First half of the wave
		if (i < half) {
			square50->setSample(i, -1);
			square25->setSample(i, -1);
			triangle->setSample(i, (((float) i / 4) - 1));
		}
		//Third quarter of the wave
		else if (i < three_fourths) {
			square50->setSample(i, 1);
			square25->setSample(i, -1);
			triangle->setSample(i, -(((float) i / 4) - 1));
		}
		//Fourth quarter of the wave
		else {
			square50->setSample(i, 1);
			square25->setSample(i, 1);
			triangle->setSample(i, -(((float) i / 4) - 1));
		}
	}
	int rnd;
	float pi = 3.14159265359;
	for (int i = 0; i < 256; i++) {
		vibrato->setSample(i, sin((pi * (float) i) / 256));
		rnd = ((-2) * ((float) rand() / RAND_MAX)) + 1;
		noise->setSample(i, rnd);
	}
	return tables;
}

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

	//TODO: generate default voices
	//TODO: Assign voices downstream
	//TODO: Fix Voice Configuration Reader

	/*** Make all Audio Connections ***/

	// The master mixer for the whole synth
	printf("Generating mixer\n");
	chip::Mixer * masterMixer = new chip::Mixer(bufferSize, sampleRate);
	// All of the wavetables for the chipophone
	printf("Generating wavetables\n");
	std::vector<chip::Wavetable *> * tables = GenerateChipTables();

	printf("Generating voices\n");
	std::vector<chip::Voice *> * voices = new std::vector<chip::Voice *>;

	printf("Setting voice defaults\n");
	// Set defaults for voices
	for (int i = 0; i < numModules; i++) {
		voices->push_back(new chip::Voice);
		voices->at(i)->arpTime = 100;
		voices->at(i)->arp_en = false;
		voices->at(i)->glissTime = 100;
		voices->at(i)->gliss_en = false;
		voices->at(i)->osc_attack = 100;
		voices->at(i)->osc_decay = 200;
		voices->at(i)->osc_sustain = .6;
		voices->at(i)->osc_release = 500;
		voices->at(i)->osc_table = tables->at(i);
		voices->at(i)->vib_en = true;
		voices->at(i)->vib_attack = 100;
		voices->at(i)->vib_decay = 200;
		voices->at(i)->vib_sustain = .6;
		voices->at(i)->vib_release = 500;
		voices->at(i)->vib_table = tables->at(5);
		voices->at(i)->volume = -3;

	}
	printf("Generating modules\n");
	// Modules
	std::vector<chip::Module *> * modules = new std::vector<chip::Module *>(0);
	for (int i = 0; i < numModules; i++) {
		modules->push_back(new chip::Module(bufferSize, sampleRate));
		modules->at(i)->setVoice(voices->at(i));
		PMHandler->addMIDIDevice(modules->at(i));
		masterMixer->addAudioDevice(modules->at(i));
	}

	/*** Make all MIDI Connections ***/

	// Print incoming MIDI Connections
	if (verbose) {
		chip::MessagePrinter * printer = new chip::MessagePrinter();
		PMHandler->addMIDIDevice(printer);
	}

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

	while (1) {
		// Read MIDI, forward
		PMHandler->readMIDI();
		masterMixer->cleanup();
	}

	PAHandler->disconnectAudioStream();
	PMHandler->disconnectMIDIStream();

}

