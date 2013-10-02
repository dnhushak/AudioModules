#include "PolyVoice.hpp"

using namespace chip;

PolyVoice::PolyVoice()
{
	note = 0;
	phase = 0;
    frequency = 0;
    state = 0;
    
    wavetable = Wavetables::getInstance();
}

float PolyVoice::getSample()
{
    int waveType = TRIANGLE;
    int phase_truncated = 16-POWER;
    float sample;
    sample = wavetable->getSample(waveType, ((int)phase)>>(phase_truncated));
    
    switch(state)
    {
        case ATTACK:
            envmult += Aslope;
            
            if(envloc >= Asamp) 
            {
                state = DECAY;
                envloc = 0;
            }
            break;
            
        case DECAY:
            envmult += Dslope;
            
            if(envloc >= Dsamp) 
            {
                state = SUSTAIN;
                envloc = 0;
            }
            break;
            
        case SUSTAIN:
            envmult = sustain;
            break;
            
        case RELEASE:
            
            break;
            
        default:
            break;
    }
    
    envloc++;
    
    return sample * envmult;
}

/*
 * 
 */
std::vector<float> PolyVoice::advance(int numSamples) 
{
    std::vector<float>* samples = new std::vector<float>(numSamples);
    float sample;
    
    //int shifted = ((int)phase)>>(phase_truncated);
    
    //std::cout << "SHIFTED: " << shifted << "\n";
    
    for(int i = 0; i < numSamples; i++)
    {
        sample = getSample();
        (*samples)[i] = (sample);
        
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

void PolyVoice::setVoice(int attack, int decay, float sustain, int release)
{
    this->attack = attack;
    this->decay = decay;
    this->sustain = sustain;
    this->release = release;
    
    envmult = 0;
    envloc = 0;
    
    Asamp = (attack * SAMPLE_RATE) / 1000;
    Aslope = 1.0 / Asamp;
    
    Dsamp = (decay * SAMPLE_RATE) / 1000;
    Dslope = (sustain - 1.0) / Dsamp;
}
