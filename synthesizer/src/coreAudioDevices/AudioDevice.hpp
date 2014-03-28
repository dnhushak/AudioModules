//Audio Device Class
#pragma once
#include <vector>
#include <iostream>
#include <cstdlib>
#include "Device.hpp"

namespace synth {
	class AudioDevice : public virtual Device {
		public:
			AudioDevice();

			// The advance call used to fill a buffer
			virtual float * advance(int);

			// Perform whatever cleanup is necessary
			virtual void cleanup();

			// Resize the buffer of the audio device
			void resizeBuffer(int);

			// Change the sample rate of the audio device
			void changeSampleRate(int);

			// Set every value in the buffer to 0
			void zeroBuffer();

			// Return the size of the buffer
			int getBufferSize();
//
//			// Get the state
//			devState_t getState();

			// Return the sample rate
			int getSampleRate();

			virtual ~AudioDevice();

		protected:
			// Size of buffer
			int bufferSize;
			// The buffer itself
			float * buffer;

			// Sampling rate of the device
			int sampleRate;

	};
}

