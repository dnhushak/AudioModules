#include "MIDIParser.hpp"

namespace chip 
{
    MIDIParser::MIDIParser()
    {
        modules = new std::vector<Module*>(0);
    }
    
    void MIDIParser::doAction(PmEvent data) 
    {
	    int status = Pm_MessageStatus(data.message);
	    //Finds channel. %4 is in there currently to fold everything down to the first four channels
	    //TODO: Handle more channels
		int channel = (status & 0x0F) %4;
		int message = status >> 4;
	    int note = Pm_MessageData1(data.message);
	    
	    // note on: (velocity > 0)
	    // note off: (velocity == 0)
	    int velocity = Pm_MessageData2(data.message);
        
        //Check for note on message
        if(message==0x9){
        	//Checks for velocity = 0 for note on message
        	if(velocity == 0)
        	{
           		(*modules)[channel]->releasePolyVoice(note);
	    	}
	    	else
	    	{
	    	    (*modules)[channel]->activatePolyVoice(note);
	    	}
	    	
	    	return;
	    }
	    //Checks for note off message
	    if(message==0x8){
	        (*modules)[channel]->releasePolyVoice(note);
	    	return;
	    }
    }
    
    void MIDIParser::interpretMIDI(int devID) 
    {
	    int i;
	    PmError retval;
	    //const PmDeviceInfo *info;
	    PmEvent msg[32];
	    PortMidiStream *mstream;
	    
	    retval = Pm_OpenInput(&mstream, devID, NULL, 512L, NULL, NULL);
	
		if(retval != pmNoError) {
			printf("error: %s \nValid ports:\n", Pm_GetErrorText(retval));
				int i;
				for (i = 0; i < Pm_CountDevices(); i++) {
        		const PmDeviceInfo *info = Pm_GetDeviceInfo(i);
        		if (info->input){
        			printf("Input:  %d: %s, %s\n", i, info->interf, info->name);
        		}
    		}
		}
	    else {
			printf("Bound to port %d\n", devID);
		    while(1) {
			    if(Pm_Poll(mstream)) {
				    int cnt = Pm_Read(mstream, msg, 32);
				    for(i=0; i<cnt; i++) {
					    doAction(msg[i]);					
				    }
			    }
		    }
	    }
	    Pm_Close(mstream);
	    return;
    }
    
    void MIDIParser::readMIDI(int devID) 
    {
	    int cnt;

	    Pm_Initialize();
	    cnt = Pm_CountDevices();
	
	    if(cnt) {
		    interpretMIDI(devID);
	    }
	    else {
		    printf("No MIDI devices found\n");
	    }
	    Pm_Terminate();
	    return;
    }
    
    void MIDIParser::addObject(Module* module)
    {
        modules->push_back(module);
    }
}
