#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include "portaudio.h"
#include "portmidi.h"
#include "utils.h"
#include "params.h"

//PolyVoice struct
typedef struct {
	//Phase register
	unsigned short phase;

	//Frequency in Hz of of this particular polyVoice
	int frequency;

	//Indicates whether or not the polyVoice is occupied; 0 for free, nonzero for occupied
	int isActive;

	//ADSR Info
	int env_mult;
	int env_loc;
	int env_state;

	//MIDI note number of polyVoice
	int note;

	//Envelope state
	int envState;

	//Envelope register
	int env;
} polyVoice;

//Initialization of modules
polyVoice module[NUM_MODULES][NUM_POLYVOICES];

//Wave tables - tri is triangle, sq1 is a 50% duty cycle, sq2 is a 25%, nse is a noise
volatile short tri[TABLE_SIZE];
volatile short sq1[TABLE_SIZE];
volatile short sq2[TABLE_SIZE];
volatile short nse[TABLE_SIZE];

//Initialize Polyvoices within a module
void polyVoice_init(polyVoice module[]) {
	int i;
	for (i = 0; i < NUM_POLYVOICES; i++) {
		module[i].phase = 0;
		module[i].frequency = 0;
		module[i].isActive = 0;
		module[i].note = 0;
		module[i].envState = 0;
		module[i].env = 0;
	}
}

//Initialize Modules
void module_init(int verbose) {
	if (verbose) printf("Initializing Modules...\n");
	int i;
	for (i = 0; i < NUM_MODULES; i++) {
		polyVoice_init(module[i]);
	}
	if (verbose) printf("...Done\n");
}

//Generate Waves
void wavetablegen(int verbose) {
	if (verbose) printf("Generating Wavetables...\n");

	int i;

	//Divide TABLE_SIZE into four regions
	int quarter = TABLE_SIZE / 4;
	int half = TABLE_SIZE / 2;
	int three_fourths = 3 * quarter;

	//Cycle through the entirety of TABLE_SIZE and generate triangle, and square waves
	//The triangle wave statement takes the current index of the for loop, casts it to a float (to do division), and scales it to do the correct math in triangle wave generation
	for (i = 0; i < TABLE_SIZE; i++) {
		//First half of the wave
		if (i < half) {
			tri[i] = -16384 + (((float) i / quarter) * 16384);
			sq1[i] = -16384;
			sq2[i] = -16384;
		}
		//Third quarter of the wave
		else if (i < three_fourths) {
			tri[i] = 16383 - ((((float) i - half) / quarter) * 16384);
			sq1[i] = 16383;
			sq2[i] = -16384;
		}
		//Fourth quarter of the wave
		else {
			tri[i] = 16384 - ((((float) i - half) / quarter) * 16384);
			sq1[i] = 16383;
			sq2[i] = 16383;
		}
		nse[i] = rand() % 16384 - 16384;
	}
	if (verbose) printf("...Done\n");
}

//Phase stepsize calculation from frequency
unsigned int stepsize(int freq) {
	//Maximum value of phase scale (16^4 in this case)
	int step;

	//Our equation!
	step = (freq * PHASESCALE) / SAMPLE_RATE;
	return step;
}

/* This routine will be called by the PortAudio engine when audio is needed.
 ** It may called at interrupt level on some machines so don't do anything
 ** that could mess up the system like calling malloc() or free().
 */
static int patestCallback(const void *inputBuffer, void *outputBuffer,
		unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo,
		PaStreamCallbackFlags statusFlags, void *userData) {
	polyVoice *data = (polyVoice*) userData;
	float *out = (float*) outputBuffer;
	unsigned long i;
	int j;
	int k;

	(void) timeInfo; /* Prevent unused variable warnings. */
	(void) statusFlags;
	(void) inputBuffer;

	long frameVal;
	int phase_truncated = 16 - POWER;

	for (i = 0; i < framesPerBuffer; i++) {
		frameVal = 0;
		//for (k=0; k<NUM_MODULES;k++){
		//Lookup the wave value of the current phase (Only uses the top 16-POWER bits of phase, to allow for table sizes smaller than the phase register)
		for (j = 0; j < NUM_POLYVOICES; j++) {
			if (module[0][j].isActive) {
				frameVal += (sq2[(module[0][j].phase) >> (phase_truncated)]);
				/* Advance Phase */
				module[0][j].phase += stepsize(module[0][j].frequency);
			}
			if (module[1][j].isActive) {
				frameVal += (sq1[(module[1][j].phase) >> (phase_truncated)]);
				/* Advance Phase */
				module[1][j].phase += stepsize(module[1][j].frequency);
			}
			if (module[2][j].isActive) {
				frameVal += (tri[(module[2][j].phase) >> (phase_truncated)]);
				/* Advance Phase */
				module[2][j].phase += stepsize(module[2][j].frequency);
			}
		}
		//}

		// Set the next element of the ARRAY (that's what *out++ does) to the fameVal / 65536
		*out++ = (float) frameVal / 65536;

	}
	return paContinue;
}

/*
 * This routine is called by portaudio when playback is done.
 */
static void StreamFinished(void* userData) {
	polyVoice *data = (polyVoice *) userData;
}

/*******************************************************************/
void doAction(PmEvent data, int verbose) {
	int status = Pm_MessageStatus(data.message);

	//Get the channel number out of status by masking the 4 most significant bits - the mod 4 is to just cycle down channels higher than 4
	int channel = (status & 0x0F) % 4;
	int message = status >> 4;
	int note = Pm_MessageData1(data.message);
	int velocity = Pm_MessageData2(data.message);

	if (verbose) {
		printf("status:%d, byte1=%d, byte2=%d, time=%.3f\n",
				Pm_MessageStatus(data.message), Pm_MessageData1(data.message),
				Pm_MessageData2(data.message), data.timestamp / 1000.0);
	}

	int i;
	int j = 0;
	for (i = 0; i < NUM_POLYVOICES; i++) {
		if (module[channel][i].note == note) {
			//New note sets j=1, prevents later for loop from occurring
			j = 1;
			module[channel][i].frequency = MtoF(note);
			module[channel][i].note = note;
			module[channel][i].isActive = velocity;

			//Check for separate note off status (most keyboard just send note on with velocity = 0)
			if (status == 0x80) {
				module[channel][i].isActive = 0;
			}
		}
	}
	if (j != 1) {
		for (i = 0; i < NUM_POLYVOICES; i++) {
			if (module[channel][i].isActive == 0) {
				module[channel][i].frequency = MtoF(note);
				module[channel][i].note = note;
				module[channel][i].isActive = velocity;
				return;
			}
		}
	}
	return;
}

void interpretMIDI(int devID, int verbose) {
	int i;
	PmError retval;
	const PmDeviceInfo *info;
	PmEvent msg[32];
	PortMidiStream *mstream;

	Pt_Start(devID, NULL, NULL);
	retval = Pm_OpenInput(&mstream, devID, NULL, 512L, NULL, NULL);


	if (retval != pmNoError) {
		printf("error: %s \n", Pm_GetErrorText(retval));
	} else {
		if (verbose) {
			printf("Bound to port %d, awaiting input:\n", devID);
		}
		while (1) {
			if (Pm_Poll(mstream)) {
				int cnt = Pm_Read(mstream, msg, 32);
				for (i = 0; i < cnt; i++) {
					doAction(msg[i], verbose);
				}
			}
		}
	}
	Pm_Close(mstream);
	return;
}

void readMIDI(int devID, int verbose) { // Reads the MIDI input stream

	int cnt;

	Pm_Initialize();
	cnt = Pm_CountDevices();
	int i;

	if (cnt) {
		interpretMIDI(devID, verbose);
	} else {
		printf("No MIDI devices found\n");
	}
	Pm_Terminate();
	return;
}

/*******************************************************************/

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
	if (verbose) printf("Device ID is: %d\n", devID);

	//Generate wavetables and initialize everything
	wavetablegen(verbose);
	module_init(verbose);

	PaStreamParameters outputParameters;
	PaStream *stream;
	PaError err;

	err = Pa_Initialize();
	if (err != paNoError)
		goto error;

	outputParameters.device = Pa_GetDefaultOutputDevice(); /* default output device */
	if (outputParameters.device == paNoDevice) {
		fprintf(stderr, "Error: No default output device.\n");
		goto error;
	}
	outputParameters.channelCount = 1; /* stereo output */
	outputParameters.sampleFormat = paFloat32; /* 32 bit floating point output */
	outputParameters.suggestedLatency = Pa_GetDeviceInfo(
			outputParameters.device)->defaultLowOutputLatency;
	outputParameters.hostApiSpecificStreamInfo = NULL;

	//Open Audio Stream
	err = Pa_OpenStream(&stream, NULL, /* no input */&outputParameters,
	SAMPLE_RATE, FRAMES_PER_BUFFER, paClipOff, patestCallback, module[0]);
	if (err != paNoError)
		goto error;

	err = Pa_SetStreamFinishedCallback(stream, &StreamFinished);
	if (err != paNoError)
		goto error;

	//Start Stream
	err = Pa_StartStream(stream);
	if (err != paNoError)
		goto error;

	readMIDI(devID, verbose);

	//Stop stream
	err = Pa_StopStream(stream);
	if (err != paNoError)
		goto error;

	//Close stream
	err = Pa_CloseStream(stream);
	if (err != paNoError)
		goto error;

	//Terminate Portaudio
	Pa_Terminate();

	return err;
	error: Pa_Terminate();
	fprintf( stderr, "An error occured while using the portaudio stream\n");
	fprintf( stderr, "Error number: %d\n", err);
	fprintf( stderr, "Error message: %s\n", Pa_GetErrorText(err));
	return err;
}
