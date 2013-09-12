
#pragma once

#include <stdio.h>
#include <math.h>
#include <iostream>
#include <vector>
#include "portaudio.h"
#include "Mixer.hpp"
#include "Module.hpp"
#include "Wavetables.hpp"

//NUM-SECONDS is simply a playback mechanism
#define NUM_SECONDS   (1)

//Sample rate of the soundcard
#define SAMPLE_RATE   (32000)

//Audio buffer size
#define FRAMES_PER_BUFFER  (256)

//Power of wavetable size (wavetable size = 2 ^ POWER)
#define POWER (4)
#define TABLE_SIZE   (1<<POWER)

//Pi, for generation of sine table
#ifndef M_PI
#define M_PI  (3.14159265)
#endif


namespace chip
{

    class AudioProcessor
    {
        public:
            AudioProcessor();
            ~AudioProcessor();
            
            Mixer* masterMixer;
            std::vector<IAudio*> modules;
            
            std::vector<float> advance(int);
            
            //void generateWavetables();
            
    };

} 

