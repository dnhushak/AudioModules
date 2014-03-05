#include "AudioProcessor.hpp"
#include "MIDIParser.hpp"
#include <algorithm>
#include "utils.h"

static chip::MIDIParser* midiParser;

// Buffer for sound data to be sent off through the audio callback
static std::vector<float> buffer;

/* This routine will be called by the PortAudio engine when audio is needed.
 ** It may called at interrupt level on some machines so don't do anything
 ** that could mess up the system like calling malloc() or free().
 */
static int paCallback(const void *inputBuffer, void *outputBuffer,
		unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo,
		PaStreamCallbackFlags statusFlags, void *userData);

/*
 * This function gets called whenever an error occurred while setting up
 * PortAudio.
 */
int errorPortAudio(int err) {
	Pa_Terminate();
	fprintf( stderr, "An error occured while using the portaudio stream\n");
	fprintf( stderr, "Error number: %d\n", err);
	fprintf( stderr, "Error message: %s\n", Pa_GetErrorText(err));
	return err;
}

/*
 * This routine is called by portaudio when playback is done.
 */
static void StreamFinished(void* userData) {
}

/*
 * Sets up the PortAudio audio stream and then waits until the user presses
 * enter to terminate. During this time, audio callbacks are occurring.
 */
int main(int argc, char *argv[]) {
	int i;
	int devID = 0;
	int verbose = 0;
	extern char *optarg;
	extern int optind, opterr;
	int ch;
	//Scans for argument inputs: -p # binds chipophone to MIDI Port number #, -v makes chipophone behave in verbose mode
	while ((ch = getopt(argc, argv, "dvp:")) != EOF) {
		switch (ch) {
			case 'p':
				if (is_int(optarg)) {
					devID = atoi(optarg);
				} else {
					fprintf(stderr,
							"Port takes an integer argument. Specify MIDI Devices to be used");
					exit(2);
				}
				break;
			case 'v':
				verbose = 1;
				printf("Executing in verbose mode...\n");
				break;
			case 'd':
				printMidiDevices();
				exit(0);
				break;
		}

	}
	if (verbose) {
		printf("Device ID is: %d\n", devID);
	}
	PaStreamParameters outputParameters;
	PaStream *stream;
	PaError err;

	chip::AudioProcessor* audioProcessor = new chip::AudioProcessor(BUFFER_SIZE, NUM_MODULES);
	midiParser = new chip::MIDIParser();

	// Give the MIDIParser pointers to the modules
	for (int i = 0; i < NUM_MODULES; i++) {
		midiParser->addObject((*(audioProcessor->modules))[i]);
	}

	err = Pa_Initialize();
	if (err != paNoError)
		return errorPortAudio(err);

	outputParameters.device = Pa_GetDefaultOutputDevice();
	//outputParameters.device = 0;
	std::cout << Pa_GetDeviceInfo(outputParameters.device)->name;
	std::cout << "\n";
	if (outputParameters.device == paNoDevice)
		return errorPortAudio(err);

	outputParameters.channelCount = NUM_AUDIO_CHANNELS;
	outputParameters.sampleFormat = paFloat32; /* 32 bit floating point output */
	outputParameters.suggestedLatency = Pa_GetDeviceInfo(
			outputParameters.device)->defaultLowOutputLatency;
	outputParameters.hostApiSpecificStreamInfo = NULL;

	err = Pa_OpenStream(&stream, NULL /* no input */, &outputParameters,
	SAMPLE_RATE,
	BUFFER_SIZE,
	paNoFlag, /* we won't output out of range samples so don't bother clipping them */
	paCallback, audioProcessor); // We want to pass a pointer to the AudioProcessor

	if (err != paNoError)
		return errorPortAudio(err);

	err = Pa_SetStreamFinishedCallback(stream, &StreamFinished);
	if (err != paNoError)
		return errorPortAudio(err);

	// Connect to the MIDI stream and start reading
	midiParser->connectToMIDIStream(devID);

	err = Pa_StartStream(stream);
	if (err != paNoError)
		return errorPortAudio(err);

	while (1) {
		/*Pa_Sleep(1000);
		 std::cout << "============\n";
		 for (int i = 0; i < audioProcessor->modules->size(); i++) {
		 audioProcessor->modules->at(i)->printPolyVoices();
		 }*/
		midiParser->readMIDI();
		audioProcessor->cleanup();
	}

// Block the front end until someone hits enter
// We are getting audio callbacks while this is happening
	std::cin.ignore(255, '\n');

	err = Pa_StopStream(stream);
	if (err != paNoError)
		errorPortAudio(err);

	midiParser->disconnectMIDIStream();

	err = Pa_CloseStream(stream);
	if (err != paNoError)
		errorPortAudio(err);

	Pa_Terminate();
}

static int paCallback(const void *inputBuffer, void *outputBuffer,
		unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo,
		PaStreamCallbackFlags statusFlags, void *userData) {

	(void) inputBuffer;
	(void) timeInfo;
	(void) statusFlags;
	chip::AudioProcessor * audio = (chip::AudioProcessor*) userData;
	float *out = (float*) outputBuffer;
	audio->advance(framesPerBuffer);
//out = audio->advance(framesPerBuffer);
	for (int i = 0; i < (int) framesPerBuffer; i++) {
		*out++ = 0;
	}

	return paContinue;
}

