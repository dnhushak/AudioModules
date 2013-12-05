#include "SongboxMidiParser.hpp"

namespace chip 
{

int SongboxMidiParser::errorPortMIDI(PmError err)
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

void SongboxMidiParser::printMIDI(int message, int data1, int data2)
{
    std::cout << "MIDI message: " <<
                " Message = " << message <<
                " Data1 = " << data1 <<
                " Data2 = " << data2 <<
                "\n";
    return;
}

/* Refer to MIDI spec 
 * http://www.midi.org/techspecs/midimessages.php
 */
void SongboxMidiParser::interpretMIDI(PmEvent data) 
{
    int status = Pm_MessageStatus(data.message);
    
    //Finds channel. %4 is in there currently to fold everything down to the first four channels
    int message = status >> 4;
    //int channel = (status & CHANNEL_MASK); //(status & CHANNEL_MASK) %4;
	
    int data1 = Pm_MessageData1(data.message); // between 0 and 127 inclusive
    int data2 = Pm_MessageData2(data.message); // between 0 and 127 inclusive
    
    printMIDI(message, data1, data2); // TODO remove this after testing

    if(message==NOTE_ON)
    {
        // TODO If recording, record note and time
    }
    else if(message==NOTE_OFF)
    {
        // TODO If recording, record note and time
    }
    else if(message==SONG_SELECT)
    {
        // TODO 
    }
    else if(message==PLAY)
    {
        // TODO Start playing the recorded song or the default song (if there is one)
        // TODO If playing a song, do nothing
        // TODO If recording a song, stop the recording and play the recording from the beginning
    }
    else if(message==PAUSE)
    {
        // TODO If playing a song, pause the playback
    }
    else if(message==STOP)
    {
        // TODO If playing a song, stop the playback
        // TODO If recording, stop the recording
    }
    else if(message==RECORD)
    {
        // TODO If recording, stop recording
        // TODO If playing a song, record over the current recording? Figure this out
    }
    else if(message==CHANGE_TEMPO)
    {
        // TODO If playing a song, adjust tempo
    }
    else
    {
        //printMIDI(message, data1, data2);
    }

    return;
}

void SongboxMidiParser::readMIDI(PortMidiStream* mstream, PmEvent* msg) 
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

int SongboxMidiParser::connectToMIDIStream(int devID)
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
}
