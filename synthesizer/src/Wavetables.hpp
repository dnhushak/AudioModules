#pragma once

#include <stdio.h>
#include <math.h>

namespace chip
{

    class Wavetables
    {
        public:
            Wavetables();
            ~Wavetables();
			float getSample(WaveEnum, float, int);
			
        private:
			//5x16 table
            
    };  
    
}
