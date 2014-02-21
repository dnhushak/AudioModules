/*
 * printAudioDevices.c
 *
 *  Created on: Feb 19, 2014
 *      Author: dnhushak
 */

#include<portaudio.h>
#include<stdio.h>

int main() {
	Pa_Initialize();
	int i;
	int ndev;
	ndev = Pa_GetDeviceCount();
	for (i = 0; i < ndev; i++) {
		const PaDeviceInfo * info = Pa_GetDeviceInfo((PaDeviceIndex) i);
		if (info->maxOutputChannels > 0) {

			if (info->maxInputChannels > 0) {
				printf("Input/Output Device:    ");
			} else {
				printf("Output Device:          ");
			}
		} else {
			if (info->maxInputChannels > 0) {
				printf("Input Device:           ");
			} else {
				printf("Device:                 ");
			}

		}
		printf("%d: %s\n", i, info->name);
	}

	PaDeviceIndex defaultin = Pa_GetDefaultInputDevice();
	PaDeviceIndex defaultout = Pa_GetDefaultOutputDevice();
	const PaDeviceInfo * inputinfo = Pa_GetDeviceInfo(
			(PaDeviceIndex) defaultin);
	const PaDeviceInfo * outputinfo = Pa_GetDeviceInfo(
			(PaDeviceIndex) defaultout);
	printf("Default Input Device:   %d: %s\n", defaultin, inputinfo->name);
	printf("Default Output Device:  %d: %s\n", defaultout, outputinfo->name);

}
