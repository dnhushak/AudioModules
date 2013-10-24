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
            //PolyVoice(func* fp);
            PolyVoice();
            ~PolyVoice() {}
            
            int note;
			unsigned short phase;
            int frequency;
            
            // State of the polyvoice
            int state;
            int getState();
            
            std::vector<float> advance(int);
            unsigned int stepsize();
            
            // Sets the state of the polyvoice to release
            void releasePolyVoice();
            
            // Sets the ADSR parameters for this polyvoice
            void setVoice(int, int, float, int);
            
            // Gets and sets the envelope multiplier
            float getEnvmult();
            void setEnvmult(float);
            
            // Gets and sets the envelope location
            float getEnvloc();
            void setEnvloc(float);
            
        private:
            Wavetables* wavetable;
            
            // ADSR envelope parameters
            int attack;
            int decay;
            float sustain;
            int release;
            
            // Function pointer to remove this polyvoice from the modules active polyvoices
            func* removeThis;
            
            float envmult;  // Envelope multiplier
            float envloc;   // Envelope location
            
            int Asamp; // Length (in samples) of the attack
            float Aslope; // Slope of the attack curve
            
            int Dsamp; // Length (in samples) of the decay
            float Dslope; // Slope of the decay curve
            
            int Rsamp; // Length (in samples) of the release
            float Rslope; // Slope of the release curve
            
            float getSample();
    };
    
}
