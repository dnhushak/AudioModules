#pragma once
#include "MIDIDevice.hpp"
#include <stdio.h>
#include <cstdlib>
#include <iostream>

namespace midi {
	
	class MessagePrinter: public MIDIDevice {
		public:
			MessagePrinter();

			virtual void affect(MIDIMessage *);

			void decodeMessageType(MIDIMessage *);
	};

}

