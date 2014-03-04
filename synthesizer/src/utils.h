/*
 * utils.h
 *
 *  Created on: Oct 12, 2013
 *      Author: dnhushak
 */

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "portmidi.h"
#include "porttime.h"

int is_int(char const* p);
int printMidiDevices(void);
double MtoF(int note);
