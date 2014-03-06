//Audio Object Interface
#pragma once
#include <vector>
#include <iostream>

namespace chip {
	class AudioDevice {
		public:
			// The advance call used to fill a buffer
			virtual float * advance(int) = 0;

			void resizeBuffer(int);

			void changeSampleRate(int);

			// Set every value in the buffer to 0
			void zeroBuffer();

			int getBufferSize();

			int getSampleRate();

			//virtual ~AudioDevice();
		protected:
			int bufferSize;
			float * buffer;

			int sampleRate;
	};
}

