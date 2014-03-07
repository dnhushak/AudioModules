#include "AudioProcessor.hpp"

namespace chip {
	AudioProcessor::AudioProcessor(int bufferSize, int sampleRate,
			int numModules) {
		masterMixer = new chip::Mixer(bufferSize, sampleRate);
		modules = new std::vector<Module*>(0);
		masterMixer->setDeviceList((std::vector<AudioDevice *> *) modules);

		// Create the 5 modules for the synthesizer and add them to the mixer
		for (int i = 0; i < numModules; i++) {
			modules->push_back(new Module(bufferSize, sampleRate));
		}
	}

	float * AudioProcessor::advance(int numSamples) {
		return masterMixer->advance(numSamples);
	}

	void AudioProcessor::cleanup() {
		for (int i = 0; i < modules->size(); i++) {
			(*modules)[i]->cleanup();
		}
	}

}
