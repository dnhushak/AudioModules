#include "PortAudioHandler.h"
#include "PortMIDIHandler.h"
#include "Mixer.h"
#include "ChannelFilter.h"
#include "Wavetable.h"
#include "Oscillator.h"
#include "MessagePrinter.h"
#include <vector>
#include "Voice.h"
#include "VCO.h"
#include "Ramp.h"
#include "Delay.h"
#include "Gain.h"
#include <math.h>
#include <unistd.h>
#include <stdlib.h>
#include "Clipper.h"
#include "AudioDevice.h"
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
		samp = (sin((pi * 2 * (float) i) / 256));
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

//	device::Device * device = new device::Device();
//	device::Device * dupeDevice = device->clone();
//
//	std::cout << "\n\n";
//	std::cout << "Device 1 ID: ";
//	std::cout << device->getDevID();
//	std::cout << "\n\n";
//	std::cout << "Device 2 ID: ";
//	std::cout << dupeDevice->getDevID();
//	std::cout << "\n\n";

	audio::Gain * gain = new audio::Gain();

	std::vector<audio::Wavetable *> * tables = GenerateSynthTables();

	audio::Oscillator * sin = new audio::Oscillator();
	sin->setWavetable(tables->at(5));
	sin->setBaseFrequency(4);

	audio::Ramp * ramp = new audio::Ramp();
	ramp->setTime(3000);
	ramp->connectDevice(sin);

	audio::VCO * wobble = new audio::VCO();
	wobble->setWavetable(tables->at(3));
	wobble->connectDevice(ramp);
	wobble->setSensitivity(.4);
	wobble->setBaseFrequency(400);

	audio::Delay * dly = new audio::Delay();
	dly->connectDevice(gain);
	dly->setDelayTime(500);

	audio::Delay * dly2 = new audio::Delay();
	dly2->connectDevice(gain);
	dly2->setDelayTime(1000);

	audio::Delay * dly3 = new audio::Delay();
	dly3->connectDevice(gain);
	dly3->setDelayTime(1500);

	audio::Mixer * mixer = new audio::Mixer();
	mixer->connectDevice(gain);

	audio::Mixer * mixer2 = mixer->clone();
	audio::Mixer * mixer3 = mixer->cloneAndConnect();
//	mixer->connectDevice(dly);
//	mixer->connectDevice(dly2);
//	mixer->connectDevice(dly3);

	gain->setGain(-12);
	gain->connectDevice(wobble);

//	audio::Gain * gain2 = gain->clone();
	/*** Set up the PA Handler. This is where the audio callback is ***/

//	audio::sample_t * buffer;
//
//	audio::AudioDevice::endOfBuffer();
//	buffer = sin->read();
//	printf("\n\nBuffer:\n");
//	for (int i = 0; i < gain->getBufferSize(); i++) {
//		printf("%d||%d\n", i, buffer[i]);
//	}
//
//	printf("\n\n");
	PaError paerr;
	paerr = PAHandler->connectAudioStream(AudioOutDevID, AudioInDevID,
			numOutChannels, numInChannels, mixer3);
	if (paerr != paNoError) {
		std::cout << "Port Audio Error";
		exit(0);
	}
	usleep(1000000);
	ramp->startRamp();
//	for (int i = 100; i < 900; i++) {
////		sin->setBaseFrequency(i);
//		usleep(10000);
//	}
//	gain->setGain(-128);
	std::cout << "\nChipophone running, press enter to end program\n";
	std::cin.ignore(255, '\n');

}
