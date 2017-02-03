#pragma once
#include "MIDIDevice.h"
#include <stdio.h>
#include <cstdlib>
#include <iostream>

namespace midi {
	using namespace std;
	
	class MessagePrinter: public MIDIDevice {
		public:
			MessagePrinter();

			virtual void process(MIDIMessage *);

			void decodeMessageType(MIDIMessage *);
	};

}

