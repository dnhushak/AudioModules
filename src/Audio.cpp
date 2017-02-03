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
#include "Connectable.h"
#include "PolyphonicHandler.h"

std::vector<audio::Wavetable *> * GenerateSynthTables() {
	std::vector<audio::Wavetable *> * tables = new std::vector<
			audio::Wavetable *>;

	std::cout << "\n Sawtooth Wavetable";
	audio::Wavetable* sawtooth = new audio::Wavetable(16);
	std::cout << "\n Triangle Wavetable";
	audio::Wavetable* triangle = new audio::Wavetable(16);
	std::cout << "\n Square 50 Wavetable";
	audio::Wavetable* square50 = new audio::Wavetable(16);
	std::cout << "\n Square 25 Wavetable";
	audio::Wavetable* square25 = new audio::Wavetable(16);
	std::cout << "\n Noise Wavetable";
	audio::Wavetable* noise = new audio::Wavetable(8192);
	std::cout << "\n Vibrasin Wavetable";
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
		sawtooth->setSample(
				i,
				(audio::sample_t) (audio::sampleMax * (((float) i / 8) - 1)));

		//First half of the wave
		if (i < half) {
			square50->setSample(i, audio::sampleMin);
			square25->setSample(i, audio::sampleMin);
			triangle->setSample(
					i, audio::sampleMin + (i / (quarter)) * audio::sampleMax);
		}
		//Third quarter of the wave
		else if (i < three_fourths) {
			square50->setSample(i, audio::sampleMax);
			square25->setSample(i, audio::sampleMin);
			triangle->setSample(
					i,
					audio::sampleMax
							- ((i - half) / (quarter)) * audio::sampleMax);
		}
		//Fourth quarter of the wave
		else {
			square50->setSample(i, audio::sampleMax);
			square25->setSample(i, audio::sampleMax);
			triangle->setSample(
					i,
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

	using namespace audio;
	using namespace midi;
	using namespace std;

	printf("Generating PortMIDI Handler...\n");
	midi::PortMIDIHandler * PMHandler = new midi::PortMIDIHandler();
	printf("Generating PortAudio Handler...\n");
	audio::PortAudioHandler * PAHandler = new audio::PortAudioHandler();

	int numOutChannels = 1;
	int numInChannels = 0;
	int numModules = 5;
	int MIDIDevID = 0;
	int AudioOutDevID = 1;
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
	std::vector<audio::Wavetable *> waveTables = *GenerateSynthTables();


	std::cout << "\n Main Oscillator";
	audio::Oscillator *osc = new Oscillator();
	osc->setWavetable(waveTables[0]);
	osc->setBaseFrequencyMIDI(50);


	std::cout << "\n First Gain";
	audio::Gain * gain = new Gain();
	gain->setGain(-12);

	gain->connectDevice(osc);


	std::cout << "\n Gain & Oscillator Clones";
	audio::Gain * gain2 = gain->clone(2);

	gain->disconnectAllDevices();
	osc->erase(2);

	audio::Mixer * mixer = new Mixer();

	device::PolyphonicHandler * poly = new device::PolyphonicHandler();

	PaError paerr;
	paerr = PAHandler->connectAudioStream(AudioOutDevID, AudioInDevID,
											numOutChannels, numInChannels,
											mixer);
	if (paerr != paNoError) {
		std::cout << "Port Audio Error";
		exit(0);
	}

	int mushroom[26];
	mushroom[0]=36;
	mushroom[1]=31;
	mushroom[2]=36;
	mushroom[3]=40;
	mushroom[4]=43;
	mushroom[5]=48;
	mushroom[6]=43;
	mushroom[7]=32;
	mushroom[8]=36;
	mushroom[9]=39;
	mushroom[10]=44;
	mushroom[11]=39;
	mushroom[12]=44;
	mushroom[13]=48;
	mushroom[14]=51;
	mushroom[15]=56;
	mushroom[16]=51;
	mushroom[17]=34;
	mushroom[18]=38;
	mushroom[19]=41;
	mushroom[20]=46;
	mushroom[21]=41;
	mushroom[22]=46;
	mushroom[23]=60;
	mushroom[24]=53;
	mushroom[25]=58;

	device::Parameter * gainVal1 = new device::Parameter();
	device::Parameter * gainVal2 = new device::Parameter();

//	usleep(1000000);
	gainVal1->setParam(0,-12.0);
	gainVal2->setParam(0,-128.0);

	printf("\nGainVal1: %f", gainVal1->getParam(0).f);
	printf("\nGainVal1: %f", gainVal1->getParam(0).d);
	printf("\nGainVal1: %i", gainVal1->getParam(0).i);
	printf("\nGainVal2: %f", gainVal2->getParam(0).f);
	printf("\nGainVal2: %f", gainVal2->getParam(0).d);
	printf("\nGainVal2: %i", gainVal2->getParam(0).i);
	printf("\n");


	for (int j = 0; j < 1; j++) {
		gain2->alter(0,*gainVal1);
		((Oscillator*) gain2->front())->setWavetable(waveTables[j]);
		for (int i = 0; i < 26; i++) {
			osc->setBaseFrequencyMIDI(i);
			((Oscillator*) gain2->front())->setBaseFrequencyMIDI(mushroom[i]+24);
			usleep(35000);
		}
		gain2->alter(0,*gainVal2);
		usleep(500000);
	}

	device::Device * osc2 = gain2->front();
	std::cout << "\n Gain 2 DevID: "<< gain2->getDevID();
	gain->connectDevice(gain2);
	gain->erase(2);
//	osc2->erase(2);





//	std::cout << "\nChipophone running, press enter to end program\n";
//	std::cin.ignore(255, '\n');

}
