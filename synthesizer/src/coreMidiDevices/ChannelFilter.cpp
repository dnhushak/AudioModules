#include "ChannelFilter.hpp"

namespace synth {
	
	ChannelFilter::ChannelFilter(int initChannel) {
		// Channel of -1 indicates omni (passes all messages)
		channel = -1;
		// Expects channel in  bit value (0-15)
		if (initChannel > -2 && initChannel < 16) {
			channel = initChannel;
		}
	}

	void ChannelFilter::affect(MIDIMessage * message) {
		// Check to see if the message is of the desired channel
		if (message->type == SYSTEM) {
			//System messages are channel independent
		} else {
			if (channel == -1 || message->channel == channel) {
				//TODO: Make this an iterator
				// If it is, forward it to all MIDI Devices
				for (int i = 0; i < numMIDIDevices; i++) {
					(*MIDIDeviceList)[i]->affect(message);
				}
			}
		}
	}

	void ChannelFilter::setChannel(int newChannel) {
		// Expects channel in  bit value (0-15)
		if (newChannel > -2 && newChannel < 16) {
			channel = newChannel;
		}
	}

}
