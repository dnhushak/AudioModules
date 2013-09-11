#pragma once

#include <stdio.h>
#include <math.h>

//Power of wavetable size (wavetable size = 2 ^ POWER)
#define POWER (4)
#define TABLE_SIZE  (1<<POWER)
#define PHASESCALE ((1<<17)-1)
#define NUM_SECONDS (1)

// Types of waves
#define SQUARE   0
#define PULSE    1
#define TRIANGLE 2
#define SAWTOOTH 3
#define NOISE    4

#define NUM_WAVES 5


namespace chip
{
class Wavetables
{
    private:
        static bool instanceFlag;
        static Wavetables *single;
        
        float table[NUM_WAVES][TABLE_SIZE];
        
        Wavetables()
        {
            //private constructor
            instanceFlag = false;
            single = NULL;
            
            wavetableGen();
        }
        
        void wavetableGen();
        
    public:
        static Wavetables* getInstance() 
        {
            if(!instanceFlag)
            {
                single = new Wavetables();
                instanceFlag = true;
                return single;
            }
            else
            {
                return single;
            }
        }
        
        ~Wavetables()
        {
            instanceFlag = false;
        }
        
        
        /*
         * Takes the rounded phase and type of table and returns the sample of those
         * indices from the wavetable.
         */
        float getSample(int waveType, int phase);
};  
}
