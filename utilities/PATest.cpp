/*
 * PATest.c++
 *
 *  Created on: Feb 20, 2014
 *      Author: dnhushak
 */

#include<portaudio.h>
#include<stdio.h>
#include<string.h>
#include<stdint.h>
#define SAMPLE_RATE (44100)
#define BUFFER_SIZE (256)
#define END (3)
#define CHANNEL_COUNT (2)
//1 for float, 2 for 16 int
#define FMT (1)


#if FMT ==1
#define BASE_AMPLITUDE (.2)
#elif FMT ==2
#define BASE_AMPLITUDE (10000)
#endif

int PaCallback(const void *inputBuffer, void *outputBuffer,
		unsigned long bufferSize, const PaStreamCallbackTimeInfo* timeInfo,
		PaStreamCallbackFlags statusFlags, void *userData);

int main() {

	//Error tracker
	PaError err;

	//Init
	err = Pa_Initialize();

	//Stream time start
	PaTime start;

	//Setting Default Output Device
	PaDeviceIndex defaultout = Pa_GetDefaultOutputDevice();
	const PaDeviceInfo * outputinfo = Pa_GetDeviceInfo(
			(PaDeviceIndex) defaultout);

	//Print Device Info
	printf("Default Output Device:  %d: %s\n", defaultout, outputinfo->name);
	printf("Output Channel Count :  %d\n", outputinfo->maxOutputChannels);

	//Setting Stream Parameters

	PaStreamParameters outparam;
	//Default all stream parameters to 0
	memset(&outparam, 0, sizeof(PaStreamParameters));
	outparam.device = defaultout;
	outparam.channelCount = CHANNEL_COUNT;
#if FMT == 1
	outparam.sampleFormat = paFloat32;
#elif FMT == 2
	outparam.sampleFormat = paInt16;
#endif
	outparam.suggestedLatency =
			Pa_GetDeviceInfo(outparam.device)->defaultLowOutputLatency;

	//Initialize and open the stream
	PaStream * outStream;

	err = Pa_OpenStream(&outStream, NULL, &outparam,
			(unsigned long) SAMPLE_RATE, (int) BUFFER_SIZE,
			paNoFlag, PaCallback, NULL);
	if (err != paNoError) {
		goto error;
	}
	err = Pa_StartStream(outStream);
	if (err != paNoError) {
		goto error;
	}

	start = Pa_GetStreamTime(outStream);

	while (Pa_GetStreamTime(outStream) <= start + END) {

	}

	err = Pa_StopStream(outStream);
	if (err != paNoError) {
		goto error;
	}
	err = Pa_CloseStream(outStream);
	if (err != paNoError) {
		goto error;
	}
	err = Pa_Terminate();
	if (err != paNoError) {
		goto error;
	}
	if (err != paNoError) {
		error: Pa_Terminate();
		fprintf( stderr, "An error occured while using the portaudio stream\n");
		fprintf( stderr, "Error number: %d\n", err);
		fprintf( stderr, "Error message: %s\n", Pa_GetErrorText(err));
		fprintf( stderr, "Host Error message: %s\n",
				Pa_GetLastHostErrorInfo()->errorText);
	}
	return err;

}

int PaCallback(const void *inputBuffer, void *outputBuffer,
		unsigned long bufferSize, const PaStreamCallbackTimeInfo* timeInfo,
		PaStreamCallbackFlags statusFlags, void *userData) {

#if FMT == 1
	float * output = (float *) outputBuffer;
	float sample;
#elif FMT == 2
	int16_t * output = (int16_t *) outputBuffer;
	int16_t sample;
#endif

	unsigned long i;
	unsigned int j;

	for (i = 0; i < bufferSize; i++) {
		if (i < (BUFFER_SIZE / 2)) {
			sample = -BASE_AMPLITUDE;
		} else {
			sample = BASE_AMPLITUDE;
		}
		for (j = 0; j < CHANNEL_COUNT; j++) {
			*output++ = sample;
		}
	}

	return paContinue;

}

