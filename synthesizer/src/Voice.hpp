#pragma once

namespace chip
{
    class Voice
    {
        private:
            int attack;
            int decay;
            int sustain;
            int release;
            
        
        public:
            // Creates a neutral ADSR envelope
            Voice();
            
            // Creates an ADSR envelope of the given values
            Voice(int, int, int, int);
        
    };
}
