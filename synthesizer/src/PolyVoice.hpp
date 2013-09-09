#pragma once
#include "Wavetables.hpp"
#include "IAudio.hpp"
#include <vector>

namespace chip
{
    class PolyVoice: public IAudio
    {
        public:
            PolyVoice();
            ~PolyVoice() {}
            
            std::vector<float> advance(int);
            
        private:
            int note;
			float phase;
            float frequency;
            int isActive;
            
            Wavetables* wavetable;
    };
    
}
