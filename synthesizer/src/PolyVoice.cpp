#include "PolyVoice.hpp"

using namespace chip;

PolyVoice::PolyVoice()
{
	note = 0;
	phase = 0;
    frequency = 0.0;
    vibFreq = 0.0; // The vibrato frequency to add to the wave
    state = 0;
    waveType = SQUARE;
    
    count = 0;
    
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
    int incrementVib;
    sample = wavetable->getSample(waveType, ((int)phase)>>(phase_truncated));
    phase += stepsize();
    
    switch(state)
    {
        case ATTACK:
            envmult += Aslope;
            
            // When the evelope location has hit the number of samples, do a state transition
            if(envloc >= Asamp) 
            {
                state = DECAY;
                envloc = 0;
            }
            break;
            
        case DECAY:
            envmult += Dslope;
            
            // When the evelope location has hit the number of samples, do a state transition
            if(envloc >= Dsamp) 
            {
                state = SUSTAIN;
                envloc = 0;
            }
            break;
            
        case SUSTAIN:
            
            // Sustain won't automatically transition the state. The state will change on note release.
            // Once the envelope location has reached the vibrato delay, start vibrato.
            if(envloc > vibDelay)
            {
            
                // This is the frequency that will be added to the notes frequency to cause a vibrato.
                vibFreq = frequency * (vibAmp * sin(vibCount * (PI / 180.0)));
                
                // This is really ghetto rigged to work... should probably fix this later.
                if(count > vibPeriod)
                {
                    vibCount++;
                    count = 0;
                }
                
                count++;
            }
            
            envmult = sustain;
            break;
            
        case RELEASE:
            envmult += Rslope;
            
            // When the evelope location has hit the number of samples, do a state transition
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
	step = ((frequency + vibFreq) * PHASESCALE) / SAMPLE_RATE;
	return step;
}

void PolyVoice::setVoice(int attack, int decay, float sustain, int release, 
                         int waveType, float vibAmp, int vibPeriod, int vibDelay)
{
    this->attack = attack;
    this->decay = decay;
    this->sustain = sustain;
    this->release = release;
    this->waveType = waveType;
    this->vibAmp = vibAmp;
    this->vibPeriod = vibPeriod;
    this->vibDelay = vibDelay;
    
    envmult = 0;
    envloc = 0;
    
    Asamp = (attack * SAMPLE_RATE) / 1000;
    Aslope = 1.0 / Asamp;
    
    Dsamp = (decay * SAMPLE_RATE) / 1000;
    Dslope = (sustain - 1.0) / Dsamp;
    
    Rsamp = (release * SAMPLE_RATE) / 1000;
    
    vibCount = 0.0;
    vibFreq = 0.0;
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


