//Audio Device Class
#pragma once
#include <vector>
#include <iostream>

namespace chip {
	enum devState_t {
		ACTIVE, INACTIVE,
	};
	class AudioDevice {
		public:
			AudioDevice();

			// The advance call used to fill a buffer
			float * advance(int);

			// Perform whatever cleanup is necessary
			void cleanup();

			// Resize the buffer of the audio device
			void resizeBuffer(int);

			// Change the sample rate of the audio device
			void changeSampleRate(int);

			// Set every value in the buffer to 0
			void zeroBuffer();

			// Return the size of the buffer
			int getBufferSize();

			// Return the sample rate
			int getSampleRate();

			// Return the state
			devState_t getState();

		protected:
			// Size of buffer
			int bufferSize;
			// The buffer itself
			float * buffer;

			// Sampling rate of the device
			int sampleRate;

			devState_t state;
	};
}

