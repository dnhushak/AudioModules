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

/*
 * 
 */
std::vector<float> PolyVoice::advance(int numSamples) 
{
    std::vector<float>* samples = new std::vector<float>(numSamples);
    
    int waveType = SQUARE;
    int phase_truncated = 16-POWER;
    float sample;
    
    //int shifted = ((int)phase)>>(phase_truncated);
    
    //std::cout << "SHIFTED: " << shifted << "\n";
    
    for(int i = 0; i < numSamples; i++)
    {
        sample = wavetable->getSample(waveType, ((int)phase)>>(phase_truncated));
        (*samples)[i] = (sample);
        
        //std::cout << (*samples)[i] << "\n";
        
        //std::cout << stepsize() << "\n";                  
        phase += stepsize();
    }
    
    return *samples;
}

unsigned int PolyVoice::stepsize()
{
    //Maximum value of phase scale (16^4 in this case)
	int step;

	//Our equation!
	step = (frequency*PHASESCALE)/SAMPLE_RATE;
	return step;
}
