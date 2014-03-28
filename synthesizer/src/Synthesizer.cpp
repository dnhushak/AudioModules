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
#include "Limiter.hpp"
#include "AudioDevice.hpp"
#include "AudioEffect.hpp"
#include <pthread.h>

std::vector<synth::Wavetable *> * GeneratesynthTables() {
	std::vector<synth::Wavetable *> * tables =
			new std::vector<synth::Wavetable *>;
	synth::Wavetable* sawtooth = new synth::Wavetable(16);
	synth::Wavetable* triangle = new synth::Wavetable(16);
	synth::Wavetable* square50 = new synth::Wavetable(16);
	synth::Wavetable* square25 = new synth::Wavetable(16);
	synth::Wavetable* noise = new synth::Wavetable(1024);
	synth::Wavetable* vibrasin = new synth::Wavetable(256);

	tables->push_back(sawtooth);
	tables->push_back(triangle);
	tables->push_back(square50);
	tables->push_back(square25);
	tables->push_back(noise);
	tables->push_back(vibrasin);

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
			triangle->setSample(i, -1 + (((float) i / 4)));
		}
		//Third quarter of the wave
		else if (i < three_fourths) {
			square50->setSample(i, 1);
			square25->setSample(i, -1);
			triangle->setSample(i, 1 - ((((float) i - half) / 4)));
		}
		//Fourth quarter of the wave
		else {
			square50->setSample(i, 1);
			square25->setSample(i, 1);
			triangle->setSample(i, 1 - ((((float) i - half) / 4)));
		}
	}
	float rnd;
	for (int i = 0; i < noise->getTableSize(); i++) {
		rnd = ((-2) * ((float) rand() / RAND_MAX)) + 1;
		noise->setSample(i, rnd);
	}
	float pi = 3.14159265359;
	float samp;
	for (int i = 0; i < 256; i++) {
		samp = (sin((pi * 2 * (float) i) / 256));
		// Scale the vibrato
		samp *= .01;
		vibrasin->setSample(i, samp);
	}
	return tables;
}

// Main
int main(int argc, char *argv[]) {

	printf("Generating PortMIDI Handler...\n");
	synth::PortMIDIHandler * PMHandler = new synth::PortMIDIHandler();
	printf("Generating PortAudio Handler...\n");
	synth::PortAudioHandler * PAHandler = new synth::PortAudioHandler();

	int bufferSize = 1024;
	int sampleRate = 41000;
	int numOutChannels = 1;
	int numInChannels = 0;
	int numModules = 5;
	int MIDIDevID = 0;
	int AudioOutDevID = 2;
	int AudioInDevID = -1;
	int verbose = 0;
	extern char *optarg;
	int ch;

	//Scans for argument inputs: -p # binds synthophone to MIDI Port number #, -v makes synthophone behave in verbose mode
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

	//TODO: Fix Voice Configuration Reader

	/*** Make all Audio Connections ***/

	// The master mixer for the whole synth
	printf("Generating mixer\n");
	synth::Limiter * masterLimiter = new synth::Limiter(bufferSize, sampleRate);
	synth::Mixer * masterMixer = new synth::Mixer(bufferSize, sampleRate);
	masterLimiter->addAudioDevice(masterMixer);
	// All of the wavetables for the synthophone
	printf("Generating wavetables\n");
	std::vector<synth::Wavetable *> * tables = GeneratesynthTables();

	printf("Generating voices\n");
	std::vector<synth::Voice *> * voices = new std::vector<synth::Voice *>;

	std::vector<synth::Module *> * modules = new std::vector<synth::Module *>;

	printf("Setting voice defaults\n");
	// Set defaults for voices
	for (int i = 0; i < numModules; i++) {
		voices->push_back(new synth::Voice);
		voices->at(i)->arpTime = 100;
		voices->at(i)->arp_en = false;
		voices->at(i)->glissTime = 100;
		voices->at(i)->gliss_en = false;
		voices->at(i)->osc_attack = 10;
		voices->at(i)->osc_decay = 50;
		voices->at(i)->osc_sustain = .4;
		voices->at(i)->osc_release = 500;
		voices->at(i)->osc_table = tables->at(i);
		if (i < 4) {
			voices->at(i)->vib_en = true;
		} else {
			voices->at(i)->vib_en = false;
		}
		voices->at(i)->vib_time = 1500;
		voices->at(i)->vib_table = tables->at(5);
		voices->at(i)->volume = -12;
	}

	printf("Generating modules\n");
	// Modules
	for (int i = 0; i < numModules; i++) {
		synth::Module * newModule = new synth::Module(bufferSize, sampleRate);
		// Set voice for each module
		newModule->setVoice(voices->at(i));
		// Create a midi Channel filter
		synth::ChannelFilter * moduleFilter = new synth::ChannelFilter(i);
		// Add the module to the channel filter outputs
		moduleFilter->addMIDIDevice(newModule);
		// Add the channel filter to the PMhandler outputs
		PMHandler->addMIDIDevice(moduleFilter);
		// Add the module to the master mixer
		masterMixer->addAudioDevice(newModule);
		// Add a new module to the vector of modules
		modules->push_back(newModule);
	}


	/*** Make all MIDI Connections ***/

	// Print incoming MIDI Connections
	if (verbose) {
		synth::MessagePrinter * printer = new synth::MessagePrinter();
		PMHandler->addMIDIDevice(printer);
	}

	/*** Set up the PA Handler. This is where the audio callback is ***/
	PaError paerr;
	paerr = PAHandler->connectAudioStream(bufferSize, sampleRate, AudioOutDevID,
			AudioInDevID, numOutChannels, numInChannels, masterLimiter);
	if (paerr != paNoError) {
		exit(0);
	}

	/*** Set up the PM handler ***/
	PmError pmerr;
	pmerr = PMHandler->connectMIDIStream(MIDIDevID);
	if (pmerr != pmNoError) {
		exit(0);
	}

	std::cin.ignore(255, '\n');

	printf("Ending...\nShutting Down MIDI Stream...\n");
	PMHandler->disconnectMIDIStream();
	printf("Shutting down Audio Stream...\n");
	PAHandler->disconnectAudioStream();
	printf("Cleaning up...\n");
	delete voices;
	delete tables;
	printf("Complete\n");

}

