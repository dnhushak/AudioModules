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
#include "Envelope.h"
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

	// ***************************************** //

	std::vector<audio::Wavetable *> waveTables = *GenerateSynthTables();

	// Oscillator
	audio::Oscillator *osc = new Oscillator();
	osc->setWavetable(waveTables[3]);
	osc->setBaseFrequencyMIDI(50);

	// Polyphonic Voice Mixer
	audio::Mixer * mixer = new Mixer();

	// Gain generation
	audio::Gain * gain = new Gain();
	gain->setGain(-24);
	gain->connectDevice(mixer);

	// Envelope Generation
	Envelope * env = new Envelope();
	env->setAttack(500);
	env->setDecay(100);
	env->setSustain(.5);
	env->setRelease(1000);
	env->connectDevice(osc);

	// Polyvoice handler
	device::PolyphonicHandler<AudioDevice, AudioDevice> * poly =
			new device::PolyphonicHandler<AudioDevice, AudioDevice>();
	poly->connectDevice(env);
	poly->setUpstream(mixer);

	// PortAudio handler
	PaError paerr;
	paerr = PAHandler->connectAudioStream(AudioOutDevID, AudioInDevID,
			numOutChannels, numInChannels, gain);
	if (paerr != paNoError){
		std::cout << "Port Audio Error";
		exit(0);
	}

	device::Parameter freq1;
	device::Parameter freq2;
	device::Parameter freq3;
	device::Parameter freq4;
	device::Parameter freq5;
	device::Parameter act;
	device::Parameter deact;
	freq1.setParam(0, 440.0);
	freq2.setParam(0, 880.0);
	freq3.setParam(0, 1760.0);
	freq4.setParam(0, 300.0);
	freq5.setParam(0, 600.0);
	act.setParam(0, ACTIVE);
	deact.setParam(0, INACTIVE);

//	for (int j = 0; j < 3; j++){
//		gain2->alter(0,*freq1);
//		osc->setWavetable(waveTables[j]);
//		for (int i = 0; i < 26; i++) {
//			osc->setBaseFrequencyMIDI(i);
//			osc->setBaseFrequencyMIDI(mushroom[i]+24);
//			usleep(35000);
//		}
//		gain2->alter(0,*freq2);
//	}

	// First note
	std::cout << "\nFirst note\n";
	poly->activateVoice(0, 8, freq1);
	usleep(1000000);

	// Second note
	std::cout << "\nSecond note\n";
	poly->activateVoice(1, 8, freq2);
	usleep(1000000);

	// Third note
	std::cout << "\nThird note\n";
	poly->activateVoice(2, 8, freq3);
	usleep(1000000);

	// Change first note
	std::cout << "\nChange First note\n";
	poly->activateVoice(0, 8, freq4);
	usleep(1000000);

	// Stop third note
	std::cout << "\nStop third note\n";
	poly->deactivateVoice(2);
	usleep(1000000);

	// Stop second note
	std::cout << "\nStop second note\n";
	poly->deactivateVoice(1);
	usleep(1000000);

	// Cleanup
	std::cout << "\nCleanup\n";
	poly->cleanup();
	usleep(1000000);

	// Start third note again
	std::cout << "\nStart third note again\n";
	poly->activateVoice(2, 8, freq5);

	std::cout << "\nChipophone running, press enter to end program\n";
	std::cin.ignore(255, '\n');

}
