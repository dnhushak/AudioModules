#include "MIDIController.hpp"

namespace chip 
{

void MIDIController::printMIDI(int message, int moduleID, int data1, int data2)
{
    std::cout << "MIDI message: " <<
                " Message = " << message <<
                " Module = " << moduleID <<
                " Data1 = " << data1 <<
                " Data2 = " << data2 <<
                "\n";
    return;
}

void MIDIController::interpretMIDI(int message, int data1, int data2, chip::Module* module, int moduleID)
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
        if(data1==GLISSANDO)
        {
            bool value = (data2 >= 64);
            MIDIController::glissandoToggle(value, module);
        }
        else if(data1==GLISSANDO_SPEED)
        {
            MIDIController::glissandoSpeed(data2, module);
        }
        else if(data1==ARPEGGIO)
        {
            bool value = (data2 >= 64);
            MIDIController::arpeggioToggle(value, module);
        }
        else if(data1==ARPEGGIO_SPEED)
        {
            MIDIController::arpeggioSpeed(data2, module);
        }
        else if(data1==CHANNEL_VOLUME)
        {
            MIDIController::channelVolume(data2, module);
        }
        else
        {
            std::cout << "ERROR: Unrecognized MIDI message. ";
        }
    }
    else if(message==PROGRAM_CHANGE)
    {
        if(data1==SELECT_VOICE)
        {
            MIDIController::selectVoice(data2, module);
        }
        else
        {
            std::cout << "ERROR: Unrecognized MIDI message. ";
        }
    }
    else
    {
        std::cout << "ERROR: Unrecognized MIDI message. ";
    }

    //printMIDI(message, moduleID, data1, data2);

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

void MIDIController::glissandoToggle(bool value, chip::Module* module)
{
    module->glissando = value;
    std::cout << "Glissando = " << module->glissando << "\n";
}

void MIDIController::glissandoSpeed(int scale, chip::Module* module)
{
    int samples = MIDIController::scaleValue(scale, GLISSANDO_MIN, GLISSANDO_MAX);
    module->glissSamples = samples;
    std::cout << "Glissando speed = " << scale << "/127 => " << samples << " (min=" << GLISSANDO_MIN << ", max=" << GLISSANDO_MAX << ")\n";
}

void MIDIController::arpeggioToggle(bool value, chip::Module* module)
{
    module->arpeggio = value;
    std::cout << "Arpeggio = " << module->arpeggio << "\n";
}

void MIDIController::arpeggioSpeed(int scale, chip::Module* module)
{
    int samples = MIDIController::scaleValue(scale, ARPEGGIO_MIN, ARPEGGIO_MAX);
    module->arpsamples = samples;
    std::cout << "Arpeggio speed = " << scale << "/127 => " << samples << " (min=" << ARPEGGIO_MIN << ", max=" << ARPEGGIO_MAX << ")\n";
}

int MIDIController::scaleValue(int value, int min, int max)
{
    // Value = 0 results in returning the min
    // Value = 127 results in returning the max
    // Value between 0 and 127 returns a value scaled between min and max
    return value * (max - min)/127 + min;
}

void MIDIController::channelVolume(int intensity, chip::Module* module)
{
    float volume = intensity / 127.0;
    std::cout << "Channel volume = " << volume << " (from 0 to 1)\n";
    module->setVolume(volume);
}

void MIDIController::selectVoice(int voiceIndex, chip::Module* module)
{
    VoiceConfigReader* voices = VoiceConfigReader::getInstance();
        
    int index =  voiceIndex % voices->numVoices(); // To prevent out of bounds error
    Voice* voice = voices->getVoiceAt(index);
    
    module->setVoice(
        voice->getAttack(), 
        voice->getDecay(),
        voice->getSustain(), 
        voice->getRelease(), 
        voice->getWaveType(),
        voice->getVibAmp(),
        voice->getVibPeriod(),
        voice->getVibDelay() );
   
   std::cout << "Change to voice #" << index << " of " << voices->numVoices() << ":" <<
    " A=" << voice->getAttack() <<
    " D=" << voice->getDecay() <<
    " S=" << voice->getSustain() <<
    " R=" << voice->getRelease() <<
    " Wave=" << voice->getWaveType() << 
    " VibAmp=" << voice->getVibAmp() <<
    " VibPeriod= " << voice->getVibPeriod() <<
    " VibDelay= " << voice->getVibDelay() << "\n";
}

}
