#include "ChannelFilter.h"

namespace midi {
	
	ChannelFilter::ChannelFilter(int initChannel) {
		// Channel of -1 indicates omni (passes all messages)
		channel = -1;
		// Expects channel in  bit value (0-15)
		if (initChannel > -2 && initChannel < 16) {
			channel = initChannel;
		}
	}

	void ChannelFilter::alter(int paramNum, Parameter p) {
		switch (paramNum) {
			case 0:
				// Channel
				setChannel(p.getParam().i);
				break;
		}
	}

	ChannelFilter * ChannelFilter::clone(int){
		// TODO: ChannelFilter clone
		return this;
	}

	void ChannelFilter::process(MIDIMessage * message) {
		if (channelMatches(message->channel) || message->statusType == SYSTEM) {
			// If it is, forward it to all MIDI Devices
			for (deviceIter = begin();deviceIter != end();deviceIter++) {
				(*deviceIter)->process(message);
			}
		}
	}

	void ChannelFilter::setChannel(int newChannel) {
		// Expects channel in  bit value (0-15)
		if (newChannel > -2 && newChannel < 16) {
			channel = newChannel;
		}
	}

	int ChannelFilter::channelMatches(int channelToCheck) {
		return (channel == -1 || channelToCheck == channel);
	}

}
