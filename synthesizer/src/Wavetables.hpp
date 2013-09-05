#pragma once

#include <stdio.h>
#include <math.h>

namespace chip
{

    class Wavetables
    {
        public:
            Wavetables(int waveType);
            ~Wavetables();
			float getSample(enum, float, int);
			int waveType;
			
        private:
			float[5][16];//wave table
            void wavetablegen(void);
    };  
    
}
