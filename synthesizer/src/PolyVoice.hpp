#pragma once
#include <stdio.h>
#include <math.h>
#include <vector>
//#include "Wavetables.hpp"
#include "IAudio.hpp"

namespace chip
{
    class PolyVoice: public IAudio
    {
        public:
            PolyVoice();
            ~PolyVoice();
            
            std::vector<float> advance(int);
        
        private:
            int note;
			float phase;
            float frequency;
            int isActive;
    };
    
}
