#include "ChannelFilter.hpp"

namespace modules {
	
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
		if (message->statusType == SYSTEM) {
			//System messages are channel independent
		} else {
			if (channelMatches(message->channel)) {
				// If it is, forward it to all MIDI Devices
				deviceIter = begin();
				while(deviceIter != end()) {
					(*deviceIter)->affect(message);
					deviceIter++;
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

	int ChannelFilter::channelMatches(int channelToCheck){
		return (channel == -1 || channelToCheck == channel);
	}

}
