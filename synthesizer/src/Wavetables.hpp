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
