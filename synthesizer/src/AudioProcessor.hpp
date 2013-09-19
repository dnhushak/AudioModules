
#pragma once

#include <stdio.h>
#include <math.h>
#include <iostream>
#include <vector>
#include "portaudio.h"
#include "Mixer.hpp"
#include "Module.hpp"
#include "Wavetables.hpp"
#include "PolyVoice.hpp"


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
            std::vector<Module*>* modules;
            
            std::vector<float> advance(int);
            
            //void generateWavetables();
            
    };

} 

