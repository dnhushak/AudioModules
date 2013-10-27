#include "MIDIParser.hpp"

namespace chip 
{

MIDIParser::MIDIParser()
{
    modules = new std::vector<Module*>(0);
}

int MIDIParser::errorPortMIDI(PmError err)
{
    int i;
    fprintf(stderr, "error: %s \nValid ports:\n", Pm_GetErrorText(err));
    for (i = 0; i < Pm_CountDevices(); i++) 
    {
	    const PmDeviceInfo *info = Pm_GetDeviceInfo(i);
	    if (info->input)
	    {
		    fprintf(stderr, "Input:  %d: %s, %s\n", i, info->interf, info->name);
	    }
    }
    return i;
}

/* Refer to MIDI spec 
 * http://www.midi.org/techspecs/midimessages.php
 */
void MIDIParser::interpretMIDI(PmEvent data) 
{
    int status = Pm_MessageStatus(data.message);
    
    //Finds channel. %4 is in there currently to fold everything down to the first four channels
    //TODO: Handle more channels
	int message = status >> 4;
    int channel = (status & CHANNEL_MASK); //(status & CHANNEL_MASK) %4;
	
    int data1 = Pm_MessageData1(data.message); // between 0 and 127 inclusive
    int data2 = Pm_MessageData2(data.message); // between 0 and 127 inclusive
    
    if(message==NOTE_ON)
    {
        int note = data1;
        int velocity = data2;
        
    	if(velocity != 0)
    	{
    	    (*modules)[channel]->activatePolyVoice(note);
    	}
    	else
    	{
    	    // NOTE_ON and velocity == 0 means turn note off
       		(*modules)[channel]->releasePolyVoice(note);
    	}
    	return;
    }
    
    if(message==NOTE_OFF)
    {
        int note = data1;
        (*modules)[channel]->releasePolyVoice(note);
    	return;
    }
    
    if(message==CONTROL_CHANGE)
    {
        if(data1==TOGGLE_GLISSANDO)
        {
            return; // TODO toggle glissando
        }
        
        if(data1==TOGGLE_ARPEGGIO)
        {
            return; // TODO toggle arpeggio
        }
        
        // TODO and so on...
        
        return;
    }
    
    if(message==PROGRAM_CHANGE)
    {
        // TODO process program changes
        return;
    }
}

void MIDIParser::readMIDI(PortMidiStream* mstream, PmEvent* msg) 
{
    while(1) 
    {
	    if(Pm_Poll(mstream)) 
	    {
	        int i;
		    int cnt = Pm_Read(mstream, msg, 32);
		    for(i=0; i<cnt; i++) 
		    {
			    interpretMIDI(msg[i]);					
		    }
	    }
    }
    return;
}

int MIDIParser::connectToMIDIStream(int devID)
{
    PmError err = Pm_Initialize();
    if( err != pmNoError ) this->errorPortMIDI(err);
    
    int count = Pm_CountDevices();
    if(count == 0) fprintf(stderr, "No MIDI devices found\n");
    
    PmEvent msg[32];
    PortMidiStream *mstream;
    
    err = Pm_OpenInput(&mstream, devID, NULL, 512L, NULL, NULL);
    if(err != pmNoError) this->errorPortMIDI(err);
    
    printf("Bound to port %d\n", devID);
    this->readMIDI(mstream, msg);

    Pm_Close(mstream);
    if( err != pmNoError ) this->errorPortMIDI(err);
    
    Pm_Terminate();
    return err;
}

void MIDIParser::addObject(Module* module)
{
    modules->push_back(module);
}

}
