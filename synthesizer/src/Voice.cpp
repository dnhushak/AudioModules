#include "Voice.hpp"

chip::Voice::Voice()
{
    attack = 1;
    decay = 1;
    sustain = 1;
    release = 1;
}

chip::Voice::Voice(int attack, int decay, int sustain, int release)
{
    this->attack = attack;
    this->decay = decay;
    this->sustain = sustain;
    this->release = release;
}
