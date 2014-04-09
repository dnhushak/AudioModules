#include "AudioDevice.hpp"

namespace synth {
	AudioDevice::AudioDevice() {
		buffer = (sample_t *) malloc(sizeof(sample_t) * bufferSize);
		zeroBuffer();
	}

	// Default behavior for audio device, just returns 0
	sample_t * AudioDevice::advance(int numSamples) {
		return buffer;
	}

	// Perform cleanup on all devices in audio device list
	void AudioDevice::cleanup() {
		audIter = deviceMan.begin();
		while (audIter != deviceMan.end()) {
			((AudioDevice *) (*audIter))->cleanup();
		}
		return;
	}

	// Set every value in buffer to 0
	void AudioDevice::zeroBuffer() {
		for (int i = 0; i < bufferSize; i++) {
			buffer[i] = 0.0;
		}
	}

	// Resize the buffer of the audio device
	void AudioDevice::setBufferSize(int newSize) {
		if (newSize > 0) {
			bufferSize = newSize;
			buffer = (sample_t *) realloc(buffer,
					sizeof(sample_t) * bufferSize);
			audIter = deviceMan.begin();
			while (audIter != deviceMan.end()) {
				((AudioDevice *) (*audIter))->setBufferSize(bufferSize);
			}
		}

	}

	// Return the buffer size
	int AudioDevice::getBufferSize() {
		return bufferSize;
	}

	// Change the sample rate of the device
	void AudioDevice::setSampleRate(int newRate) {
		if (newRate > 0) {
			sampleRate = newRate;
		}
	}

	// Return the audio sampling rate
	int AudioDevice::getSampleRate() {
		return sampleRate;
	}

	AudioDevice::~AudioDevice() {
		free(buffer);
	}

	void AudioDevice::addAudioDevice(AudioDevice * newAudioDevice) {
		deviceMan.addDevice(newAudioDevice);
	}
	void AudioDevice::addAudioDevice(int loc, AudioDevice * newAudioDevice) {
		deviceMan.addDevice(loc, newAudioDevice);
	}
	void AudioDevice::addAudioDevices(
			std::list<AudioDevice *> * newAudioDeviceList) {
		deviceMan.addDevices((std::list<Device *> *)newAudioDeviceList);
	}

	// Replacing AudioDevices
	void AudioDevice::replaceAudioDevice(int loc,
			AudioDevice * newAudioDevice) {
		deviceMan.replaceDevice(loc, newAudioDevice);
	}
	void AudioDevice::replaceAudioDevice(AudioDevice * oldAudioDevice,
			AudioDevice * newAudioDevice) {
		deviceMan.replaceDevice(oldAudioDevice, newAudioDevice);
	}

	// Removing AudioDevices
	void AudioDevice::removeAudioDevice(AudioDevice * removeAudioDevice) {
		deviceMan.removeDevice(removeAudioDevice);
	}

	void AudioDevice::removeAudioDevice(int loc) {
		deviceMan.removeDevice(loc);
	}
	void AudioDevice::removeAllAudioDevices() {
		deviceMan.removeAllDevices();
	}

	int AudioDevice::hasSpace() {
		return deviceMan.hasSpace();
	}
	int AudioDevice::getNumAudioDevices() {
		return deviceMan.getNumDevices();
	}
	int AudioDevice::getMaxNumAudioDevices() {
		return deviceMan.getMaxNumDevices();
	}
	void AudioDevice::setMaxNumAudioDevices(int newMax) {
		deviceMan.setMaxNumDevices(newMax);
	}
}
