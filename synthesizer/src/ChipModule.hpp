#ifndef CHIPMODULE_HPP_
#define CHIPMODULE_HPP_

#include "coreMidiDevices/MIDIDevice.hpp"
#include "synthesisTools/PolyModule.hpp"

namespace synth {
	
	class ChipModule: public synth::MIDIDevice, public synth::PolyModule {
		public:
			ChipModule(int, int);

			void affect(MIDIMessage *);

			float * advance(int);

			~ChipModule();
		private:
			ChipVoice * voice;
			bool arp_en;
			bool gliss_en;
			int arpTime;
			int glissTime;

			synth::Gain * ChipModuleGain;
	};

}

#endif /* CHIPMODULE_HPP_ */
