#pragma once

#include <stdio.h>
#include <math.h>

//Power of wavetable size (wavetable size = 2 ^ POWER)
#define POWER (4)
#define TABLE_SIZE  (1<<POWER)
#define PHASESCALE ((1<<17)-1)
#define NUM_SECONDS (1)

// Types of waves
#define NUM_WAVES 5
#define SQUARE   0
#define PULSE    1
#define TRIANGLE 2
#define SAWTOOTH 3
#define NOISE    4

namespace chip
{
    class Wavetables
    {
        public:
            Wavetables();
            ~Wavetables();
			float getSample(int waveType, float phase, int numSamples);
			float table[NUM_WAVES][TABLE_SIZE]; //wave table
            
        private:
			void wavetablegen(void);
    };  
}
