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
    
    MIDIController::interpretMIDI(message, data1, data2, (*modules)[channel]);
    return;
}

/*
PMEXPORT PmError Pm_OpenOutput( PortMidiStream** stream,
PmDeviceID outputDevice,
void *outputDriverInfo,
int32_t bufferSize,
PmTimeProcPtr time_proc,
void *time_info,
int32_t latency ); 
*/
void MIDIParser::outputMIDI(int devID)
{
	PmError retval;
	PortMidiStream *mstream;
	PmEvent msg[32];
	int i = 0;

	retval = Pm_OpenOutput(&mstream, devID, NULL, 512L, NULL, NULL, 0);

	if(retval != pmNoError)
	{
		printf("error: %s \nValid ports:\n", Pm_GetErrorText(retval));
		int i;
		for (i = 0; i < Pm_CountDevices(); i++) 
		{
			const PmDeviceInfo *info = Pm_GetDeviceInfo(i);
			if (info->input)
			{
				printf("Input:  %d: %s, %s\n", i, info->interf, info->name);
			}
		}
	}
	else 
	{
		printf("Bound to port %d\n", devID);
		while(1) 
		{
			if(Pm_Poll(mstream)) //CHANGE THIS
	    		{
				int cnt = Pm_Read(mstream, msg, 32); //CHANGE THIS
		    		for(i=0; i<cnt; i++) 
		    		{
					//doAction(msg[i]); //CHANGE THIS					
		    		}
	    		}
		}
	}

	retval = Pm_Write( &mstream, msg, 32 );

	Pm_Close(mstream);
	return;
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
