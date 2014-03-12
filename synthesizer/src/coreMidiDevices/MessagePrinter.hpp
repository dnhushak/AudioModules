#pragma once
#include "MIDIDevice.hpp"
#include <stdio.h>

namespace chip {
	
	class MessagePrinter: public MIDIDevice {
		public:
			MessagePrinter();

			virtual void affect(MIDIMessage *);

			void decodeMessageType(MIDIMessage *);
	};

}

