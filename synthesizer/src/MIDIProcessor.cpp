#include "MIDIProcessor.hpp"

namespace chip
{

MIDIProcessor::MIDIProcessor()
{

}

/* Refer to MIDI spec 
 * http://www.midi.org/techspecs/midimessages.php
 */
void MIDIProcessor::interpretMIDI(PmEvent data)
{
    int status = Pm_MessageStatus(data.message);

    //Finds channel. %4 is in there currently to fold everything down to the first four channels
    int message = status >> 4;
    int channel = (status & CHANNEL_MASK) % 5; //(status & CHANNEL_MASK) %4;

    int data1 = Pm_MessageData1(data.message); // between 0 and 127 inclusive
    int data2 = Pm_MessageData2(data.message); // between 0 and 127 inclusive

    MIDIController::interpretMIDI(message, data1, data2, (*modules)[channel], channel);
    return;
}

void MIDIProcessor::readMIDI(PortMidiStream * mstream)
{
    while(Pm_Poll(mstream))
    {
	    int i;
		int cnt = Pm_Read(mstream, msg, 32);
		for(i=0; i<cnt; i++)
        {
            interpretMIDI(msg[i]);
	    }
    }
    return;
}


void MIDIProcessor::addObject(AudioProcessor* module)
{
    modules->push_back(module);
}

}
