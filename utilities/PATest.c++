/*
 * PATest.c++
 *
 *  Created on: Feb 20, 2014
 *      Author: dnhushak
 */

#include<portaudio.h>
#include<stdio.h>
#include<string.h>


static int PaStreamCallback(const void *inputBuffer, void *outputBuffer,
		unsigned long bufferSize, const PaStreamCallbackTimeInfo* timeInfo,
		PaStreamCallbackFlags statusFlags, void *userData);

int main() {
	PaError err;
	err = Pa_Initialize();

	PaDeviceIndex defaultout = Pa_GetDefaultOutputDevice();
	const PaDeviceInfo * outputinfo = Pa_GetDeviceInfo(
			(PaDeviceIndex) defaultout);
	printf("Default Output Device:  %d: %s\n", defaultout, outputinfo->name);
	PaStreamParameters outparam;

	memset(&outparam, 0, sizeof(PaStreamParameters));
	outparam.device = defaultout;
	outparam.channelCount = 2;
	outparam.sampleFormat = paInt16;
	outparam.suggestedLatency =
			Pa_GetDeviceInfo(outparam.device)->defaultLowOutputLatency;
	double sampleRate = 44100;
	unsigned long bufferSize = 512;

	PaStream * outStream;
	err = Pa_OpenStream(&outStream, NULL, &outparam, sampleRate, bufferSize,
	paNoFlag,
	PaStreamCallback, NULL);
	err = Pa_StartStream(outStream);
}

static int PaStreamCallback(const void *inputBuffer, void *outputBuffer,
		unsigned long bufferSize, const PaStreamCallbackTimeInfo* timeInfo,
		PaStreamCallbackFlags statusFlags, void *userData) {

}

