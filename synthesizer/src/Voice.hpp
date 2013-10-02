#pragma once

namespace chip
{
    class Voice
    {
        private:
            int attack;
            int decay;
            float sustain;
            int release;
            
        
        public:
            // Creates a neutral ADSR envelope
            Voice();
            
            // Creates an ADSR envelope of the given values
            Voice(int, int, float, int);
        
    };
}
