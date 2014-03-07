#include "AudioProcessor.hpp"

namespace chip {
	void AudioProcessor::AudioProcessor(int bufferSize, int sampleRate,
			int numModules) {
		masterMixer = new chip::Mixer(bufferSize, sampleRate);
		audioDeviceList = new std::vector<Module*>(0);
		masterMixer->setAudioDeviceList(
				(std::vector<AudioDevice *> *) audioDeviceList);

		// Create the 5 modules for the synthesizer and add them to the mixer
		for (int i = 0; i < numModules; i++) {
			addAudioDevice(new Module(bufferSize, sampleRate));
		}
	}

	float * AudioProcessor::advance(int numSamples) {
		return masterMixer->advance(numSamples);
	}

	void AudioProcessor::cleanup() {
		for (int i = 0; i < audioDeviceList->size(); i++) {
			(*audioDeviceList)[i]->cleanup();
		}
	}

	void AudioProcessor::setVoice(Voice * newVoice, int moduleNum) {
		if (moduleNum < 0 || moduleNum > getNumAudioDevices()) {
			return;
		} else {
			(*audioDeviceList)[moduleNum]->setVoice(newVoice);
		}
	}

	void AudioProcessor::activatePolyVoice(int moduleNum, int note) {
		if (moduleNum < 0 || moduleNum > getNumAudioDevices()) {
			return;
		} else {
			(*audioDeviceList)[moduleNum]->activatePolyVoice(note);
		}
	}

	void AudioProcessor::releasePolyVoice(int moduleNum, int note) {
		if (moduleNum < 0 || moduleNum > getNumAudioDevices()) {
			return;
		} else {
			(*audioDeviceList)[moduleNum]->releasePolyVoice(note);
		}
	}
}
