#pragma once

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "chiputil.hpp"

namespace chip
{
class Wavetables
{
    private:
        static bool instanceFlag;
        static Wavetables *single;
        
        float table[NUM_WAVES][TABLE_SIZE];
        
        float vibrato[360];
        
        Wavetables()
        {
            //private constructor
            instanceFlag = true;
            wavetableGen();
        }
        
        void wavetableGen();
        
    public:
        static Wavetables* getInstance() 
        {
            if(!instanceFlag)
            {
                single = new Wavetables();
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
        
        float getVibrato(int i);
};  
}
