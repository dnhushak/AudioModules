#include "PolyVoice.hpp"

chip::PolyVoice::PolyVoice()
{
	note = 0;
	phase = 0;
    frequency = 0;
    isActive = 0;
}


std::vector<float> chip::PolyVoice::advance(int numSamples) 
{
    
    return NULL;    //TODO wavetable lookup
}
