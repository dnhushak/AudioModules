#include "MIDIController.hpp"

namespace chip 
{

void MIDIController::interpretMIDI(int message, int data1, int data2, chip::Module* module)
{
    if(message==NOTE_ON)
    {
        MIDIController::noteOn(data1, data2, module);
    }
    else if(message==NOTE_OFF)
    {
        MIDIController::noteOff(data1, module);
    }
    else if(message==CONTROL_CHANGE)
    {
        if(data1==GLISSANDO_TOGGLE)
        {
            MIDIController::glissandoToggle(module);
        }
        else if(data1==GLISSANDO_SPEED)
        {
            MIDIController::glissandoSpeed(data2, module);
        }
        else if(data1==ARPEGGIO_TOGGLE)
        {
            MIDIController::arpeggioToggle(module);
        }
        else if(data1==ARPEGGIO_SPEED)
        {
            MIDIController::arpeggioSpeed(data2, module);
        }
        else if(data1==CHANNEL_VOLUME)
        {
            MIDIController::channelVolume(data2, module);
        }
    }
    else if(message==PROGRAM_CHANGE)
    {
        if(data1==SELECT_VOICE)
        {
            MIDIController::selectVoice(data2, module);
        }
    }
    else
    {
        std::cerr << "ERROR: Unrecognized MIDI message. " <<
                "Message = " << message <<
                "Data1 = " << data1 <<
                "Data2 = " << data2 <<
                "\n";
    }

    return;
}


void MIDIController::noteOn(int note, int velocity, chip::Module* module)
{
    if(velocity != 0)
    {
        module->activatePolyVoice(note);
    }
    else
    {
        // NOTE_ON and velocity == 0 means turn note off
        noteOff(note, module);
    }
}

void MIDIController::noteOff(int note, chip::Module* module)
{ 
    module->releasePolyVoice(note);
}

void MIDIController::glissandoToggle(chip::Module* module)
{
    module->glissando = !module->glissando;
}

void MIDIController::glissandoSpeed(int scale, chip::Module* module)
{
    int samples = MIDIController::scaleValue(scale, GLISSANDO_MIN, GLISSANDO_MAX);
    module->glissSamples = samples;
}

void MIDIController::arpeggioToggle(chip::Module* module)
{
    module->arpeggio = !module->arpeggio;
}

void MIDIController::arpeggioSpeed(int scale, chip::Module* module)
{
    int samples = MIDIController::scaleValue(scale, ARPEGGIO_MIN, ARPEGGIO_MAX);
    module->arpsamples = samples;
}

int MIDIController::scaleValue(int value, int min, int max)
{
    // Value = 0 results in returning the min
    // Value = 127 results in returning the max
    // Value between 0 and 127 returns a value scaled between min and max
    return (value*(max - min))/max + min;
}

void MIDIController::channelVolume(int intensity, chip::Module* module)
{
    // TODO not implemented in Module yet.
}

void MIDIController::selectVoice(int voiceIndex, chip::Module* module)
{
    // TODO Get voice info from the Voice configuration file reader
    // TODO module->setVoice(int attack, int decay, float sustain, int release, int waveType);
}

}
