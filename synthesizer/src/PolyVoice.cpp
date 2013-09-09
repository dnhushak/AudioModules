#include "PolyVoice.hpp"

using namespace chip;

PolyVoice::PolyVoice()
{
	note = 0;
	phase = 0;
    frequency = 0;
    isActive = 0;
    
    wavetable = Wavetables::getInstance();
}

std::vector<float> PolyVoice::advance(int numSamples) 
{
    std::vector<float> temp;
    
    int waveType = SQUARE;
    float phase = 0; // ?
    
    wavetable->getSample(waveType, phase, numSamples); //TODO pass the correct values
    
    return temp;    //TODO lern2C++
}
