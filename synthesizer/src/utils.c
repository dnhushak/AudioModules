/*
 * utils.c
 *
 *  Created on: Oct 12, 2013
 *      Author: dnhushak
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "portmidi.h"
#include "porttime.h"

int is_int(char const* p) {
	char compare[10];
	sprintf(compare, "%d", atoi(p));
	return strcmp(compare, p) == 0;
}



