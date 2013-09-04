#pragma once
#include <stdio.h>
#include <math.h>
#include "Module.hpp"
#include "Wavetables.hpp"


namespace chip
{

    class PolyVoice
    {
        public:
            PolyVoice();
            ~PolyVoice();
        
        private:
            int note;
			float phase;
            float frequency;
            int isActive;
    };
    
}
