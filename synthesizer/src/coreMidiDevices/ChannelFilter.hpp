#pragma once
#include "MIDIDevice.hpp"

namespace chip {
	
	class ChannelFilter: public chip::MIDIDevice {
		public:
			//Initialize with a starting channel number
			ChannelFilter(int);

			// Pass on the filtered MIDI channel data to each item in MIDIDeviceList
			void affect(MIDIMessage *);

			// Set the new channel, checking for correct channel numbers
			void setChannel(int);
		private:
			int channel;
	};

}
