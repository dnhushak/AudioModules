#include "MIDIDevice.hpp"

namespace chip {
	
	class MessagePrinter: public MIDIDevice {
		public:
			MessagePrinter();

			void affect(MIDIMessage *);

			void decodeMessageType(MIDIMessage *);

	};

}

