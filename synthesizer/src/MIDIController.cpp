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
            //MIDIController::channelVolume(data2, module);
            
            MIDIController::selectVoice(data2, module); // TODO remove this when finished testing.
        }
        else if(data1==64) // TODO remove this when finished with testing. This is the sustain button on Jack's keyboard
        {
            MIDIController::arpeggioToggle(module);
        }
        else if(data1==1) // TODO remove this when finished with testing. This is the mod button on Jack's keyboard
        {
            //MIDIController::arpeggioToggle(module);
        }
        else
        {
            error(message, data1, data2, module);
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
            //error(message, data1, data2, module);
            
            // TODO remove this when finished with testing. This is the +/- buttons on Jack's keyboard
            MIDIController::arpeggioSpeed(data1, module); 
        }
    }
    else
    {
        error(message, data1, data2, module);
    }
    //error(message, data1, data2, module);

    return;
}

void MIDIController::error(int message, int data1, int data2, chip::Module* module)
{
    std::cout << "ERROR: Unrecognized MIDI message. " <<
                " Message = " << message <<
                " Data1 = " << data1 <<
                " Data2 = " << data2 <<
                "\n";
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
    std::cout << "Glissando = " << module->glissando << "\n";
}

void MIDIController::glissandoSpeed(int scale, chip::Module* module)
{
    int samples = MIDIController::scaleValue(scale, GLISSANDO_MIN, GLISSANDO_MAX);
    module->glissSamples = samples;
    std::cout << "Glissando speed = " << scale << "/127 => " << samples << " (min=" << GLISSANDO_MIN << ", max=" << GLISSANDO_MAX << ")\n";
}

void MIDIController::arpeggioToggle(chip::Module* module)
{
    module->arpeggio = !module->arpeggio;
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
    // TODO not implemented in Module yet.
    std::cout << "WARNING: channel volume not implemented yet. ";
    std::cout << "Channel volume = " << intensity << "\n";
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
        voice->getWaveType() );
   
   std::cout << "Change to voice #" << index << " of " << voices->numVoices() << ":" <<
    " A=" << voice->getAttack() <<
    " D=" << voice->getDecay() <<
    " S=" << voice->getSustain() <<
    " R=" << voice->getRelease() <<
    " Wave=" << voice->getWaveType() << "\n";
}

}
