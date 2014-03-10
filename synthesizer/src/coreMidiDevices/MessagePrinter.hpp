#pragma once
#include "MIDIDevice.hpp"

namespace chip {
	
	class MessagePrinter: public MIDIDevice {
		public:
			MessagePrinter();

			virtual void affect(MIDIMessage *);

			void decodeMessageType(MIDIMessage *);
	};

}

