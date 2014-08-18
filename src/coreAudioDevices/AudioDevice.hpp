//Audio Device Class
#ifndef AUDIOUDEVICE_HPP_
#define AUDIOUDEVICE_HPP_

#include "Device.hpp"
#include "AudioUtils.hpp"

// For memset, memcpy, etc..
#include <string.h>

namespace audio {
	// Size of buffer
	static int bufferSize = 64;

	// Sampling rate of the audio system
	static int sampleRate = 44100;

	class AudioDevice: public virtual device::Device {
		public:
			AudioDevice();

			// The advance call used to fill a buffer
			virtual sample_t * advance(int) = 0;

			/**
			 * Reads the buffer, generally to be used after an advance.
			 * Can be used to create "multiple outputs," where two devices read
			 * the buffer of another without needing to change the buffer
			 * @return A pointer to the buffer of samples of type ```sample_t```
			 */
			sample_t * read();

			// Perform whatever cleanup is necessary
			virtual void cleanup();

			/**
			 * Resize the buffer of the audio device
			 * @param newSize New size of buffer in samples. Must be > 0
			 */
			static void setBufferSize(int);
			/**
			 * Return the buffer size
			 * @return Number of samples making up the audio buffer
			 */
			static int getBufferSize();

			/**
			 * Change the sample rate of the device
			 * @param newRate New sample rate, in samples per second. Must be > 0
			 */
			static void setSampleRate(int);
			// Return the sample rate
			static int getSampleRate();

			virtual ~AudioDevice();

		protected:
			/**
			 * Quickly set every sample in the buffer to 0
			 */
			void zeroBuffer();

			// The buffer itself
			sample_t * buffer;

	};
}
#endif //AUDIOUDEVICE_HPP_
