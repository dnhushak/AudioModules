#pragma once

//NUM-SECONDS is simply a playback mechanism
#define NUM_SECONDS   (1)

//Sample rate of the soundcard
#define SAMPLE_RATE   (32000)

//Audio buffer size
#define FRAMES_PER_BUFFER  (1024)
#define PHASESCALE ((1<<17)-1)

//Power of wavetable size (wavetable size = 2 ^ POWER)
#define POWER (4)
#define TABLE_SIZE   (1<<POWER)

//State that the polyvoice is in
#define OFF 0
#define ATTACK 1
#define DECAY 2
#define SUSTAIN 3
#define RELEASE 4
#define CLEANUP 5

// Number of polyvoices
#define NUM_POLYVOICES 127

#define NUM_SECONDS (1)

// Types of waves
#define SQUARE   0
#define PULSE    1
#define TRIANGLE 2
#define SAWTOOTH 3
#define NOISE    4

#define NUM_WAVES 5


typedef void func(int);
