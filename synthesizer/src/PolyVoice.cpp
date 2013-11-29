#include "PolyVoice.hpp"

using namespace chip;

PolyVoice::PolyVoice()
{
	note = 0;
	phase = 0;
    frequency = 0.0;
    state = 0;
    waveType = SQUARE;
    
    wavetable = Wavetables::getInstance();
}

float PolyVoice::getSample()
{
    if(state == OFF || state == CLEANUP)
    {
        return 0.0;
    }

    int phase_truncated = 16-POWER;
    float sample;
    sample = wavetable->getSample(waveType, ((int)phase)>>(phase_truncated));
    phase += stepsize();
    
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
            envmult += Rslope;
            
            if(envloc >= Rsamp)
            {
                state = CLEANUP;
            }
            break;
            
        default:
            return 0.0;
    }
    
    envloc++;
    
    return sample * envmult;
}

int PolyVoice::getState()
{
    return state;
}

void PolyVoice::releasePolyVoice()
{
    state = RELEASE;
    envloc = 0;
    Rslope = -envmult / Rsamp;
}

std::vector<float> PolyVoice::advance(int numSamples) 
{
    std::vector<float>* samples = new std::vector<float>(numSamples);
    float sample;
    
    // Obtain the sample from the wavetable and advance the phase register
    for(int i = 0; i < numSamples; i++)
    {
        sample = getSample();
        (*samples)[i] = (sample);
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

void PolyVoice::setVoice(int attack, int decay, float sustain, int release, int waveType)
{
    this->attack = attack;
    this->decay = decay;
    this->sustain = sustain;
    this->release = release;
    this->waveType = waveType;
    
    envmult = 0;
    envloc = 0;
    
    Asamp = (attack * SAMPLE_RATE) / 1000;
    Aslope = 1.0 / Asamp;
    
    Dsamp = (decay * SAMPLE_RATE) / 1000;
    Dslope = (sustain - 1.0) / Dsamp;
    
    Rsamp = (release * SAMPLE_RATE) / 1000;
}

float PolyVoice::getEnvmult() {
    return envmult;
}

void PolyVoice::setEnvmult(float newmult) {
    envmult = newmult;
}

float PolyVoice::getEnvloc() {
    return envloc;
}

void PolyVoice::setEnvloc(float newloc) {
    envloc = newloc;
}


