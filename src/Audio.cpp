#include "PortAudioHandler.hpp"
#include "PortMIDIHandler.hpp"
#include "Mixer.hpp"
#include "ChannelFilter.hpp"
#include "Wavetable.hpp"
#include "Oscillator.hpp"
#include "MessagePrinter.hpp"
#include "Module.hpp"
#include "Voice.hpp"
#include "Gain.hpp"
#include <math.h>
#include <unistd.h>
#include <stdlib.h>
#include "Clipper.hpp"
#include "AudioDevice.hpp"
#include <pthread.h>

std::vector<audio::Wavetable *> * GenerateSynthTables() {
	std::vector<audio::Wavetable *> * tables = new std::vector<
			audio::Wavetable *>;
	audio::Wavetable* sawtooth = new audio::Wavetable(16);
	audio::Wavetable* triangle = new audio::Wavetable(16);
	audio::Wavetable* square50 = new audio::Wavetable(16);
	audio::Wavetable* square25 = new audio::Wavetable(16);
	audio::Wavetable* noise = new audio::Wavetable(8192);
	audio::Wavetable* vibrasin = new audio::Wavetable(256);

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
				(audio::sample_t) (audio::sampleMax * (((float) i / 8) - 1)));

		//First half of the wave
		if (i < half) {
			square50->setSample(i, audio::sampleMin);
			square25->setSample(i, audio::sampleMin);
			triangle->setSample(i,
					audio::sampleMin + (i / (quarter)) * audio::sampleMax);
		}
		//Third quarter of the wave
		else if (i < three_fourths) {
			square50->setSample(i, audio::sampleMax);
			square25->setSample(i, audio::sampleMin);
			triangle->setSample(i,
					audio::sampleMax
							- ((i - half) / (quarter)) * audio::sampleMax);
		}
		//Fourth quarter of the wave
		else {
			square50->setSample(i, audio::sampleMax);
			square25->setSample(i, audio::sampleMax);
			triangle->setSample(i,
					audio::sampleMax
							- ((i - half) / (quarter)) * audio::sampleMax);
		}
	}
	triangle->setSample(8, audio::sampleMax);
	float rnd;
	for (int i = 0; i < noise->getTableSize(); i++) {
		rnd = ((audio::sample_t) ((-2) * ((float) rand() / RAND_MAX)) + 1);
		noise->setSample(i, rnd);
	}
	float pi = 3.14159265359;
	float samp;
	for (int i = 0; i < 256; i++) {
		samp = (audio::sample_t) (sin((pi * 2 * (float) i) / 256));
		// Scale the vibrato
		samp *= .01;
		samp *= audio::sampleMax;
		vibrasin->setSample(i, samp);
	}
	return tables;
}

// Main
int main(int argc, char *argv[]) {

	printf("Generating PortMIDI Handler...\n");
	midi::PortMIDIHandler * PMHandler = new midi::PortMIDIHandler();
	printf("Generating PortAudio Handler...\n");
	audio::PortAudioHandler * PAHandler = new audio::PortAudioHandler();

	int numOutChannels = 1;
	int numInChannels = 0;
	int numModules = 5;
	int MIDIDevID = 0;
	int AudioOutDevID = 2;
	int AudioInDevID = -1;
	int verbose = 0;
	extern char *optarg;
	int ch;

	//Scans for argument inputs: -p # binds audioophone to MIDI Port number #, -v makes audioophone behave in verbose mode
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
				//audio::AudioDevice::setBufferSize(atoi(optarg));
				break;
			case 's':
				// Sample Rate argument
				//audio::AudioDevice::setSampleRate(atoi(optarg));
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

	audio::Gain * gain = new audio::Gain();
	printf("Gain 1:  %d\n", gain->getDevID());
	audio::Gain * gain2 = new audio::Gain();
	printf("Gain 2:  %d\n", gain2->getDevID());
	audio::Gain * gain3 = new audio::Gain();
	printf("Gain 3:  %d\n", gain3->getDevID());
	//delete gain3;
	audio::Gain * gain4 = new audio::Gain();
	printf("Gain 4:  %d\n", gain4->getDevID());

	std::vector<audio::Wavetable *> * tables = GenerateSynthTables();

	audio::Oscillator * saw = new audio::Oscillator();
	saw->setWavetable(tables->at(1));
	printf("Osc  :  %d\n", saw->getDevID());
	saw->setBaseFrequency(300);

	audio::Mixer * mixer = new audio::Mixer();
	printf("Mixer:   %d\n", mixer->getDevID());
	mixer->addDevice(gain);
	mixer->addDevice(gain2);
	mixer->addDevice(gain3);
//	printf("Mixer 1: %d\n",mixer->getDeviceByLoc(0)->getDevID());
//	printf("Mixer 2: %d\n",mixer->getDeviceByLoc(1)->getDevID());
//	printf("Mixer 3: %d\n",mixer->getDeviceByLoc(2)->getDevID());
//	printf("Mixer 6: %d\n",mixer->getDeviceByLoc(5)->getDevID());
//	printf("Mixer-2: %d\n",mixer->getDeviceByLoc(-3)->getDevID());
	mixer->addDevice(1, gain4);
//	printf("Mixer 2: %d\n",mixer->getDeviceByLoc(1)->getDevID());
//	printf("Mixer 3: %d\n",mixer->getDeviceByLoc(2)->getDevID());

	/*** Set up the PA Handler. This is where the audio callback is ***/
	PaError paerr;
	paerr = PAHandler->connectAudioStream(AudioOutDevID, AudioInDevID,
			numOutChannels, numInChannels, saw);
	if (paerr != paNoError) {
		std::cout << "Port Audio Error";
		exit(0);
	}

	for (int i = 100; i < 900; i++) {
		saw->setBaseFrequency(i);
		usleep(1000);
	}

}
