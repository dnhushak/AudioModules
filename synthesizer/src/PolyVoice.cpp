#include "PolyVoice.hpp"

using namespace chip;

PolyVoice::PolyVoice()
{

    // Initialize everything to 0 so there are no chances of scary sounds
	note = 0;
	phase = 0;
    frequency = 0.0;
    vibFreq = 0.0; // The vibrato frequency to add to the wave
    state = 0;
    waveType = SQUARE;
    
    count = 0;
    
    attack = 0;
    decay = 0;
    sustain = 0;
    release = 0;
    
    waveType = SQUARE;
    
    vibAmp = 0;
    vibPeriod = 0;
    vibDelay = 0;
    vibCount = 0;
    vibFreq = 0.0;
    
    state = OFF;
    
    envmult = 0;
    envloc = 0;
    Asamp = 0;
    Aslope = 0;
    Dsamp = 0;
    Dslope = 0;
    Rsamp = 0;
    Rslope = 0;
    
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
    
    // This is the ADSR "state machine"
    // Attack goes from 0 volume to 1
    // Decay goes from 1 to the Sustain volume
    // Release goes from the Sustain volume to 0
    // Below is a graph of a notes volume going through the states of ADSR   
    //
    //     /\
    //    /  \__________
    //   /              \
    //  /                \
    // /                  \
    // 
    // | A |D|    S    | R | 
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
                vibFreq = frequency * wavetable->getVibrato(vibCount);
                
                // This is really ghetto rigged to work... should probably fix this later.
                if(count > vibPeriod)
                {
                    vibCount = (vibCount + 1) % 360;
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
    
    vibCount = 0;
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


