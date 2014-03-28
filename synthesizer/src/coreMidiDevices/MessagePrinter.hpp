#pragma once
#include "MIDIDevice.hpp"
#include <stdio.h>

namespace synth {
	
	class MessagePrinter: public MIDIDevice {
		public:
			MessagePrinter();

			virtual void affect(MIDIMessage *);

			void decodeMessageType(MIDIMessage *);
	};

}

