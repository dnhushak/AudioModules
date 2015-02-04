#pragma once
#include "MIDIDevice.h"
#include "Connectable.h"
#include "Alterable.h"

namespace midi {
	using namespace device;
	using namespace std;
	
	class ChannelFilter: public Alterable<Connectable<MIDIDevice, MIDIDevice> >{
		public:
			//Initialize with a starting channel number
			ChannelFilter(int);

			void alter(string paramName, Parameter p);

			// Pass on the filtered MIDI channel data to each item in MIDIDeviceList
			void affect(MIDIMessage *);

			// Set the new channel, checking for correct channel numbers
			void setChannel(int);
		private:
			int channelMatches(int);
			int channel;
	};

}
