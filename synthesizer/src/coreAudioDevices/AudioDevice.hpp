//Audio Device Class
#pragma once
#include <vector>
#include <iostream>
#include <cstdlib>
#include <string.h>
#include "Device.hpp"
#include "DeviceManager.hpp"
#include "AudioUtils.hpp"

namespace synth {

	class AudioDevice: public virtual Device {
		public:
			AudioDevice();

			// The advance call used to fill a buffer
			virtual sample_t * advance(int);

			// Perform whatever cleanup is necessary
			virtual void cleanup();

			// Resize the buffer of the audio device
			static void setBufferSize(int);
			// Return the size of the buffer
			static int getBufferSize();

			// Change the sample rate of the audio device
			static void setSampleRate(int);
			// Return the sample rate
			static int getSampleRate();

			// Audio Device Management

			// Adding AudioDevices
			void addAudioDevice(AudioDevice * newAudioDevice);
			void addAudioDevice(int loc, AudioDevice * newAudioDevice);
			void addAudioDevices(std::list<AudioDevice *> * newAudioDeviceList);

			// Replacing AudioDevices
			void replaceAudioDevice(int loc, AudioDevice * newAudioDevice);
			void replaceAudioDevice(AudioDevice * oldAudioDevice, AudioDevice * newAudioDevice);

			// Removing AudioDevices
			void removeAudioDevice(AudioDevice * removeAudioDevice);
			void removeAudioDevice(int loc);
			void removeAllAudioDevices();

			int hasSpace();
			int getNumAudioDevices();
			int getMaxNumAudioDevices();
			void setMaxNumAudioDevices(int newMax);

			virtual ~AudioDevice();
		protected:
			// Size of buffer
			static int bufferSize;

			// Sampling rate of the device
			static int sampleRate;

			// Set every value in the buffer to 0
			void zeroBuffer();

			// The buffer itself
			sample_t * buffer;

			// Device Manager
			DeviceManager deviceMan;
			std::list<Device *>::iterator audIter;

	};
}

