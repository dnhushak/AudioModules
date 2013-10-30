/*
 * utils.c
 *
 *  Created on: Oct 12, 2013
 *      Author: dnhushak
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "portmidi.h"
#include "porttime.h"

int is_int(char const* p) {
	char compare[10];
	sprintf(compare, "%d", atoi(p));
	return strcmp(compare, p) == 0;
}

int printMidiDevices(void) {
	int i;
	for (i = 0; i < Pm_CountDevices(); i++) {
		const PmDeviceInfo *info = Pm_GetDeviceInfo(i);
		if (info->input) {
			printf("Input:  ");
		} else {
			printf("Output: ");
		}
		printf("%d: %s, %s", i, info->interf, info->name);
		printf("\n");
	}
	return 0;
}

//Midi Note to Frequency
double MtoF(int note) {
	return pow(2, (((double) note - 69) / 12.0)) * 440.0;
}

