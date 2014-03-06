#include "AudioProcessor.hpp"

namespace chip {
	AudioProcessor::AudioProcessor(int bufferSize, int numModules) {
		unsigned int i;
		masterMixer = new chip::Mixer(bufferSize);
		modules = new std::vector<Module*>();

		std::cout << "Creating audio processor!\n";

		// Create the first and only instance of the Wavetables.
		Wavetables* wavetable = Wavetables::getInstance();

		// Read from VoiceConfig file.
		VoiceConfigReader* voiceReader = VoiceConfigReader::getInstance();
		bool validFile = voiceReader->numVoices() >= 5;
		if (!validFile) {
			std::cout
					<< "Error reading voice config file. Not enough voices.\n";
		}

		// Create the 5 modules for the synthesizer and add them to the mixer
		for (i = 0; i < numModules; i++) {
			Module * module;
			if (validFile) {
				module = new Module(voiceReader->getVoiceAt(i));
			} else {
				module = new Module(bufferSize);
			}

			modules->push_back(module);
			masterMixer->addObjects((AudioClass*) module);
		}
	}

	std::vector<float> * AudioProcessor::advance(int numSamples) {
		return masterMixer->advance(numSamples);
	}

	void AudioProcessor::cleanup() {
		for (int i = 0; i < modules->size(); i++) {
			(*modules)[i]->cleanup();
		}
	}

	AudioProcessor::~AudioProcessor() {
	}

}
