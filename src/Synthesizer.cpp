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
#include <pthread.h>

std::vector<modules::Wavetable *> * GeneratesynthTables() {
	std::vector<modules::Wavetable *> * tables = new std::vector<
			modules::Wavetable *>;
	modules::Wavetable* sawtooth = new modules::Wavetable(16);
	modules::Wavetable* triangle = new modules::Wavetable(16);
	modules::Wavetable* square50 = new modules::Wavetable(16);
	modules::Wavetable* square25 = new modules::Wavetable(16);
	modules::Wavetable* noise = new modules::Wavetable(8192);
	modules::Wavetable* vibrasin = new modules::Wavetable(256);

	tables->push_back(sawtooth);
	tables->push_back(triangle);
	tables->push_back(square50);
	tables->push_back(square25);
	tables->push_back(noise);
	tables->push_back(vibrasin);

	//Divide 16 into four regions
	float quarter = 4;
	float half = 2 * quarter;
	float three_fourths = 3 * quarter;

	//Cycle through the entirety of 16 and generate triangle, and square waves
	//The triangle wave statement takes the current index of the for loop, casts it to a float (to do division), and scales it to do the correct math in triangle wave generation
	for (float i = 0; i < 16; i++) {
		sawtooth->setSample(i,
				(modules::sample_t) (modules::sampleMax * (((float) i / 8) - 1)));

		//First half of the wave
		if (i < half) {
			square50->setSample(i, modules::sampleMin);
			square25->setSample(i, modules::sampleMin);
			triangle->setSample(i,
					modules::sampleMin + (i / (quarter)) * modules::sampleMax);
		}
		//Third quarter of the wave
		else if (i < three_fourths) {
			square50->setSample(i, modules::sampleMax);
			square25->setSample(i, modules::sampleMin);
			triangle->setSample(i,
					modules::sampleMax
							- ((i - half) / (quarter)) * modules::sampleMax);
		}
		//Fourth quarter of the wave
		else {
			square50->setSample(i, modules::sampleMax);
			square25->setSample(i, modules::sampleMax);
			triangle->setSample(i,
					modules::sampleMax
							- ((i - half) / (quarter)) * modules::sampleMax);
		}
	}
	triangle->setSample(8, modules::sampleMax);
	float rnd;
	for (int i = 0; i < noise->getTableSize(); i++) {
		rnd = ((modules::sample_t) ((-2) * ((float) rand() / RAND_MAX)) + 1);
		noise->setSample(i, rnd);
	}
	float pi = 3.14159265359;
	float samp;
	for (int i = 0; i < 256; i++) {
		samp = (modules::sample_t) (sin((pi * 2 * (float) i) / 256));
		// Scale the vibrato
		samp *= .01;
		samp *= modules::sampleMax;
		vibrasin->setSample(i, samp);
	}
	return tables;
}

// Main
int main(int argc, char *argv[]) {

	printf("Generating PortMIDI Handler...\n");
	modules::PortMIDIHandler * PMHandler = new modules::PortMIDIHandler();
	printf("Generating PortAudio Handler...\n");
	modules::PortAudioHandler * PAHandler = new modules::PortAudioHandler();

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
				modules::AudioDevice::setBufferSize(atoi(optarg));
				break;
			case 's':
				// Sample Rate argument
				modules::AudioDevice::setSampleRate(atoi(optarg));
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
	modules::Limiter * masterLimiter = new modules::Limiter();
	modules::Mixer * masterMixer = new modules::Mixer();

	masterLimiter->addDevice(masterMixer);
	// All of the wavetables for the synthophone
	printf("Generating wavetables\n");
	std::vector<modules::Wavetable *> * tables = GeneratesynthTables();

	printf("Generating voices\n");
	std::vector<modules::Voice *> * voices = new std::vector<modules::Voice *>;

	std::vector<modules::Module *> * modules = new std::vector<modules::Module *>;

	printf("Setting voice defaults\n");
	// Set defaults for voices
	for (int i = 0; i < numModules; i++) {
		// TODO: Handle this in a voice reader class
		modules::Voice * newVoice = new modules::Voice;
		newVoice->arpTime = 100;
		newVoice->arp_en = false;
		newVoice->glissTime = 100;
		newVoice->gliss_en = false;
		newVoice->osc_attack = 10;
		newVoice->osc_decay = 10;
		newVoice->osc_sustain = 1;
		newVoice->osc_release = 10;
		newVoice->osc_table = tables->at(i);
		if (i < 4) {
			newVoice->vib_en = true;
		} else {
			newVoice->vib_en = false;
		}
		newVoice->vib_time = 1500;
		newVoice->vib_table = tables->at(5);
		newVoice->volume = -12;
		voices->push_back(newVoice);
	}

	// Legend of Zelda Bass Voice
	voices->at(2)->volume = -6;
	voices->at(2)->osc_table = tables->at(1);
	voices->at(2)->vib_en = false;
	voices->at(2)->osc_attack = 10;
	voices->at(2)->osc_decay = 10;
	voices->at(2)->osc_sustain = 1;
	voices->at(2)->osc_release = 10;

	// Zelda Countermelody
	voices->at(1)->volume = -15;
	voices->at(1)->vib_en = false;
	voices->at(1)->osc_attack = 10;
	voices->at(1)->osc_decay = 10;
	voices->at(1)->osc_sustain = 1;
	voices->at(1)->osc_release = 10;
	voices->at(1)->osc_table = tables->at(2);

	// Zelda Melody
	voices->at(0)->volume = -15;
	voices->at(0)->vib_en = false;
	voices->at(0)->osc_attack = 10;
	voices->at(0)->osc_decay = 10;
	voices->at(0)->osc_sustain = 1;
	voices->at(0)->osc_release = 10;
	voices->at(0)->osc_table = tables->at(2);

	// Zelda Drums
	voices->at(4)->volume = -15;
	voices->at(4)->vib_en = false;
	voices->at(4)->osc_attack = 10;
	voices->at(4)->osc_decay = 100;
	voices->at(4)->osc_sustain = 0;
	voices->at(4)->osc_release = 10;
	voices->at(4)->osc_table = tables->at(4);

	printf("Generating modules\n");
	// Modules
	for (int i = 0; i < numModules; i++) {
		modules::Module * newModule = new modules::Module();
		// Set voice for each module
		newModule->setVoice(voices->at(i));
		// Create a midi Channel filter
		modules::ChannelFilter * moduleFilter = new modules::ChannelFilter(i);
		// Add the module to the channel filter outputs
		moduleFilter->addDevice(newModule);
		// Add the channel filter to the PMhandler outputs
		PMHandler->addDevice(moduleFilter);
		// Add the module to the master mixer
		masterMixer->addDevice(newModule);
		// Add a new module to the vector of modules
		modules->push_back(newModule);
	}

	/*** Make all MIDI Connections ***/

	// Print incoming MIDI Connections
	if (verbose) {
		modules::MessagePrinter * printer = new modules::MessagePrinter();
		PMHandler->addDevice(printer);
	}

	/*** Set up the PA Handler. This is where the audio callback is ***/
	PaError paerr;
	paerr = PAHandler->connectAudioStream(AudioOutDevID, AudioInDevID,
			numOutChannels, numInChannels, masterLimiter);
	if (paerr != paNoError) {
		std::cout << "Port Audio Error";
		exit(0);
	}

	/*** Set up the PM handler ***/
	PMHandler->StartCallback();
	PmError pmerr;
	pmerr = PMHandler->openMIDIInput(MIDIDevID);
	if (pmerr != pmNoError) {
		std::cout << "Port MIDI Error";
		exit(0);
	}
//	while (1) {
//		PMHandler->readMIDI();
//	}
	std::cout << "\nChipophone running, press enter to end program\n";
	std::cin.ignore(255, '\n');

	//PMHandler->StopCallback();
	printf("Shutting down Audio Stream...\n");
	PAHandler->disconnectAudioStream();
	printf("Ending...\nShutting Down MIDI Stream...\n");
	PMHandler->disconnectMIDIStream();
	printf("Cleaning up...\n");
	delete voices;
	delete tables;
	printf("Complete\n");

}
