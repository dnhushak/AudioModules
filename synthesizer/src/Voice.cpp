#include "Voice.hpp"

chip::Voice::Voice()
{
    attack = 1;
    decay = 1;
    sustain = 0.5;
    release = 1;
    waveType = SQUARE;
}

chip::Voice::Voice(int attack, int decay, float sustain, int release, 
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
}

/*** Getters ***/

int chip::Voice::getAttack()
{
    return attack;
}

int chip::Voice::getDecay()
{
    return decay;
}

float chip::Voice::getSustain()
{
    return sustain;
}

int chip::Voice::getRelease()
{
    return release;
}

int chip::Voice::getWaveType()
{
    return waveType;
}

float chip::Voice::getVibAmp()
{
    return vibAmp;
}

int chip::Voice::getVibPeriod()
{
    return vibPeriod;
}

int chip::Voice::getVibDelay()
{
    return vibDelay;
}

/*** Setters ***/

void chip::Voice::setAttack(int attack)
{
    this->attack = attack;
}

void chip::Voice::setDecay(int decay)
{
    this->decay = decay;
}

void chip::Voice::setSustain(float sustain)
{
    this->sustain = sustain;
}

void chip::Voice::setRelease(int release)
{
    this->release = release;
}

void chip::Voice::setWaveType(int waveType)
{
    this->waveType = waveType;
}

void chip::Voice::setVibAmp(float vibAmp)
{
    this->vibAmp = vibAmp;
}

void chip::Voice::setVibPeriod(int vibPeriod)
{
    this->vibPeriod = vibPeriod;
}

void chip::Voice::setVibDelay(int vibDelay)
{
    this->vibDelay = vibDelay;
}
