//Audio Device Class
#pragma once
#include <vector>
#include <iostream>

namespace chip {
	class AudioDevice {
		public:
			// The advance call used to fill a buffer
			float * advance(int);

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

		protected:
			// Size of buffer
			int bufferSize;
			// The buffer itself
			float * buffer = new float[bufferSize];;

			// Sampling rate of the device
			int sampleRate;
	};
}

