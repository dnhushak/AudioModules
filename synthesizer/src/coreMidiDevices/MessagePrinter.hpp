#include "MIDIDevice.hpp"

namespace chip {
	
	class MessagePrinter: public chip::MIDIDevice {
		public:
			MessagePrinter();

			void affect(MIDIMessage *);

			void decodeMessageType(MIDIMessage *);

	};

}

