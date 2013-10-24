#pragma once
#include <sstream>
#include <string>
#include "chiputil.hpp"

namespace chip
{
    class Voice
    {
        private:
            // Attack, decay, and release are all measured in Milliseconds.
            // Sustain is measured as a percentile of volume (0.0 to 1.0)
            int attack;
            int decay;
            float sustain;
            int release;
            
            // The wave type
            int waveType;
            
        public:
            // Creates a neutral ADSR envelope with a square wave
            Voice();
            
            // Creates an ADSR envelope of the given values and the wavetype
            Voice(int, int, float, int, int);
            
            // Getters
            int getAttack();
            int getDecay();
            float getSustain();
            int getRelease();
            int getWaveType();
            
            // Setters
            void setAttack(int);
            void setDecay(int);
            void setSustain(float);
            void setRelease(int);
            void setWaveType(int);
            
            std::string toString();
    };
}

