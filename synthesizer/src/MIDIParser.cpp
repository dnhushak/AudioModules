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
	    // TODO int channel;
	    int note = Pm_MessageData1(data.message);
	    
	    // note on: (velocity > 0)
	    // note off: (velocity == 0)
	    int velocity = Pm_MessageData2(data.message);
        
        if(velocity > 0)
        {
            (*modules)[0]->activatePolyVoice(note);
	    }
	    else
	    {
	        (*modules)[0]->releasePolyVoice(note);
	    }
	    
	    return;
    }
    
    void MIDIParser::interpretMIDI() 
    {
	    int dev = 2; // dev is the device, should always be 2		
	    int i;
	    PmError retval;
	    //const PmDeviceInfo *info;
	    PmEvent msg[32];
	    PortMidiStream *mstream;

	    //Pt_Start(1, NULL, NULL);
	    retval = Pm_OpenInput(&mstream, dev, NULL, 512L, NULL, NULL);
	
	    if(retval != pmNoError) {
		    printf("error: %s \n", Pm_GetErrorText(retval));
	    }
	    else {
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
    
    void MIDIParser::readMIDI() 
    {
	    int cnt;

	    Pm_Initialize();
	    cnt = Pm_CountDevices();
	
	    if(cnt) {
		    interpretMIDI();
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
