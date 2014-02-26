#pragma once

//Audio Hardware Definitions
#define NUM_AUDIO_CHANNELS (2)
#define SAMPLE_RATE (32000)

//Audio buffer size
#define BUFFER_SIZE  (1024)
#define PHASESCALE ((1<<17)-1)

//Power of wavetable size (wavetable size = 2 ^ POWER)
#define POWER (4)
#define TABLE_SIZE (1<<POWER)

//State that the polyvoice is in
#define OFF 0
#define ATTACK 1
#define DECAY 2
#define SUSTAIN 3
#define RELEASE 4
#define CLEANUP 5

//Number of audio modules
#define NUM_MODULES 5

//Number of polyvoices
#define NUM_POLYVOICES 18

// Maximum number of active polyvoices
#define MAX_POLYVOICES 16

//Types of waves
#define SQUARE   0
#define PULSE    1
#define TRIANGLE 2
#define SAWTOOTH 3
#define NOISE    4

#define NUM_WAVES 5

//Config filenames
#define VOICE_CONFIG_FILE "VoicesConfig.txt"

//MIDI file list
#define MIDI_FILE_NAMES "../src/config/MidiFileList.txt"

// PI
#define PI 3.14159265

typedef void func(int);
