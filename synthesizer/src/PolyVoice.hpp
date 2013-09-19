#pragma once
#include "Wavetables.hpp"
#include "IAudio.hpp"
#include <vector>
#include <iostream>


//NUM-SECONDS is simply a playback mechanism
#define NUM_SECONDS   (1)

//Sample rate of the soundcard
#define SAMPLE_RATE   (32000)

//Audio buffer size
#define FRAMES_PER_BUFFER  (256)
#define PHASESCALE ((1<<17)-1)

//Power of wavetable size (wavetable size = 2 ^ POWER)
#define POWER (4)
#define TABLE_SIZE   (1<<POWER)

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
            int isActive;
            
        private:
            Wavetables* wavetable;
    };
    
}
