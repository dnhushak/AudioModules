/*
 * params.h
 *
 *  Created on: Oct 12, 2013
 *      Author: dnhushak
 */

//Sample rate of the soundcard
#define SAMPLE_RATE   (32000)

//Audio buffer size
#define FRAMES_PER_BUFFER (512)

//Number of Poly Voices per module
#define NUM_MODULES (5)
#define NUM_POLYVOICES (16)

//Power of wavetable size (wavetable size = 2 ^ POWER)
#define POWER (4)
#define TABLE_SIZE  (1<<POWER)
#define PHASESCALE ((1<<16)-1)

//ADSR States
#define INACTIVE (0)
#define ATTACK (1)
#define DECAY (2)
#define SUSTAIN (3)
#define RELEASE (4)
