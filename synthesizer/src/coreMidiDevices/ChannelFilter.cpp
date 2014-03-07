/*
 * ChannelFilter.cpp
 *
 *  Created on: Mar 7, 2014
 *      Author: dnhushak
 */

#include "ChannelFilter.hpp"

namespace chip {
	
	ChannelFilter::ChannelFilter(int initChannel) {
		// Expects channel in MIDI channel value (1-16) and not bit value (0-15)
		channel = initChannel - 1;
	}

	void ChannelFilter::affect(MIDIMessage * message){
		// Check to see if the message is of the desired channel
		if (message->channel == channel){
			// If it is, forward it to all MIDI Devices
			for (int i = 0; i< numMIDIDevices; i++){
				(*MIDIDeviceList)[i]->affect(message);
			}
		}
	}

	void ChannelFilter::setChannel(int newChannel) {
		// Expects channel in MIDI channel value (1-16) and not bit value (0-15)
		if (newChannel > 0 && newChannel < 17) {
			channel = newChannel - 1;
		}
	}

}
