#include "ChipModule.hpp"

namespace synth {
	
	ChipModule::ChipModule() {
		// TODO Auto-generated constructor stub

		PolyModuleGain = new Gain(bufferSize, sampleRate);
		PolyModuleGain->addAudioDevice(polyMixer);
	}

}
