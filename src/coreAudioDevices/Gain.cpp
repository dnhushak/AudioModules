#include "Gain.h"

namespace audio {

	Gain::Gain() {
		setMaxNumDevices(1);
		// Default gain to 0 db
		setGain(0);
	}

	Gain * Gain::clone() {
		// Create new device
		Gain * newDevice = new Gain();
		// Set all member variables
		newDevice->state = this->state;
		newDevice->gain = this->gain;

		return newDevice;
	}

	void Gain::alter(string paramName, Parameter p) {
		if (!paramName.compare("gain")) {
			setGain(p.getParam().f);
		}
	}

	sample_t * Gain::advance() {
		if (isEmpty()) {
			zeroBuffer();
		} else {
			copyToBuffer(front()->read(), bufferSize);
			for (int i = 0; i < bufferSize; i++) {
				buffer[i] *= gain;
			}
		}
		return buffer;
	}

	void Gain::process(const sample_t *inBuffer, sample_t *outBuffer,
			int samplesToProcess, int numChannels) {

		// Account for multichannel buffers
		int totalSamples = samplesToProcess * numChannels;

		// Go through each sample in the buffer
		for (int i = 0; i < totalSamples; i++) {
			outBuffer[i] = inBuffer[i] * gain;

		}
	}

	void Gain::setGain(float volume) {
		// Volume is coming in dbs
		// 0 dbs => gain of 1
		gain = dbToRatio(volume);
	}
}
