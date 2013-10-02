#pragma once
#include "Wavetables.hpp"
#include "IAudio.hpp"
#include <vector>
#include <iostream>
#include "chiputil.hpp"


namespace chip
{
    class PolyVoice: public IAudio
    {
        public:
            PolyVoice();
            ~PolyVoice() {}
            
            std::vector<float> advance(int);
            unsigned int stepsize();
            
            int note;
			unsigned short phase;
            int frequency;
            int state;
            
            float envmult;  // Envelope multiplier
            float envloc;   // Envelope location
            
        private:
            Wavetables* wavetable;
    };
    
}
