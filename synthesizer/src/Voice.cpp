#include "Voice.hpp"

chip::Voice::Voice()
{
    attack = 1;
    decay = 1;
    sustain = 0.5;
    release = 1;
    waveType = SQUARE;
}

chip::Voice::Voice(int attack, int decay, float sustain, int release, int waveType)
{
    this->attack = attack;
    this->decay = decay;
    this->sustain = sustain;
    this->release = release;
    this->waveType = waveType;
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
