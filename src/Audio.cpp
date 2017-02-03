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

std::vector<audio::Wavetable *> * GenerateSynthTables(){
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
	for (float i = 0; i < 16; i++){
		sawtooth->setSample(i,
				(audio::sample_t) (audio::sampleMax * (((float) i / 8) - 1)));

		//First half of the wave
		if (i < half){
			square50->setSample(i, audio::sampleMin);
			square25->setSample(i, audio::sampleMin);
			triangle->setSample(i,
					audio::sampleMin + (i / (quarter)) * audio::sampleMax);
		}
		//Third quarter of the wave
		else if (i < three_fourths){
			square50->setSample(i, audio::sampleMax);
			square25->setSample(i, audio::sampleMin);
			triangle->setSample(i,
					audio::sampleMax
							- ((i - half) / (quarter)) * audio::sampleMax);
		}
		//Fourth quarter of the wave
		else{
			square50->setSample(i, audio::sampleMax);
			square25->setSample(i, audio::sampleMax);
			triangle->setSample(i,
					audio::sampleMax
							- ((i - half) / (quarter)) * audio::sampleMax);
		}
	}
	triangle->setSample(8, audio::sampleMax);
	float rnd;
	for (int i = 0; i < noise->getTableSize(); i++){
		rnd = ((audio::sample_t) ((-2) * ((float) rand() / RAND_MAX)) + 1);
		noise->setSample(i, rnd);
	}
	float pi = 3.14159265359;
	float samp;
	for (int i = 0; i < 256; i++){
		samp = (sin((pi * 2 * (float) i) / 256));
		samp *= audio::sampleMax;
		vibrasin->setSample(i, samp);
	}
	return tables;
}

// Main
int main(int argc, char *argv[]){

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
	while ((ch = getopt(argc, argv, "dvp:b:s:c:m:a:")) != EOF){
		switch (ch){
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

	audio::Oscillator *osc = new Oscillator();
	osc->setWavetable(waveTables[3]);
	osc->setBaseFrequencyMIDI(50);

//	audio::Oscillator *osc2 = osc->clone(2);

	audio::Mixer * mixer = new Mixer();
//	mixer->connectDevice(osc2);

	audio::Gain * gain = new Gain();
	gain->setGain(-24);
	gain->connectDevice(mixer);

	device::PolyphonicHandler<AudioDevice, AudioDevice> * poly =
			new device::PolyphonicHandler<AudioDevice, AudioDevice>();
	poly->connectDevice(osc);
	poly->setUpstream(mixer);

	PaError paerr;
	paerr = PAHandler->connectAudioStream(AudioOutDevID, AudioInDevID,
			numOutChannels, numInChannels, gain);
	if (paerr != paNoError){
		std::cout << "Port Audio Error";
		exit(0);
	}

	device::Parameter * freq1 = new device::Parameter();
	freq1->setParam(0, 440.0);

	for (int j = 0; j < 3; j++){
//		gain2->alter(0,*freq1);
//		osc->setWavetable(waveTables[j]);
//		for (int i = 0; i < 26; i++) {
//			osc->setBaseFrequencyMIDI(i);
//			osc->setBaseFrequencyMIDI(mushroom[i]+24);
//			usleep(35000);
//		}
//		gain2->alter(0,*freq2);

		poly->activateVoice(j, *freq1);

		freq1->setParam(0, freq1->getParam().d * 2.0);

		usleep(1000000);
	}

	freq1->setParam(0, 300.);
	poly->activateVoice(0, *freq1);

	usleep(1000000);
	poly->deactivateVoice(2);
	usleep(1000000);
	poly->deactivateVoice(1);

	poly->cleanup();

	freq1->setParam(0, freq1->getParam().d * 2.0);

	usleep(1000000);
	poly->activateVoice(2, *freq1);

	std::cout << "\nChipophone running, press enter to end program\n";
	std::cin.ignore(255, '\n');

}
