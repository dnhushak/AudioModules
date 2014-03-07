#pragma once
#include <iostream>
#include <vector>
#include <unistd.h>
#include "portmidi.h"
#include "Module.hpp"
#include "MIDIController.hpp"

#define CHANNEL_MASK 0x0F

namespace chip {
	class MIDIProcessor {
		public:
			MIDIProcessor();

			void readMIDI();

			void addObject(Module* audioObject);

		private:
			PmEvent msg[32];

			std::vector<Module*>* modules;

			void interpretMIDI(PmEvent data);
	};
}
