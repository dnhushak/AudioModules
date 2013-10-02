#include "Voice.hpp"

chip::Voice::Voice()
{
    attack = 1;
    decay = 1;
    sustain = 0.5;
    release = 1;
}

chip::Voice::Voice(int attack, int decay, float sustain, int release)
{
    this->attack = attack;
    this->decay = decay;
    this->sustain = sustain;
    this->release = release;
}

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
