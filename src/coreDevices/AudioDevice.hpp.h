//Audio Device Class
#ifndef AUDIOUDEVICE_HPP_
#define AUDIOUDEVICE_HPP_

#include "Device.hpp"
#include "AudioUtils.hpp"

// For memset, memcpy, etc..
#include <string.h>
// For device list
#include <list>

namespace audio {
	// Size of buffer
	//TODO: Change buffer of all audio devices
	static int bufferSize = 64;

	// Sampling rate of the audio system
	//TODO: Ability to used mixed sample rates
	static int sampleRate = 44100;

	/**
	 * This is the master class for all audio devices.
	 * Each device has an output buffer of samples, intended to be read
	 * by either an audio output device interfacing the program with the OS's devices
	 * or another audio device within the application.
	 *
	 * Most audio devices in the library utilize the ```ConnectableDevice``` class, allowing multiple
	 * devices to be connected to each other. Typically in the ```advance``` call of one device, the ```advance```
	 * call of a connected device will also be called, to retain sample coherency and sampler-by-sample
	 * effects, such as VCAs, etc.
	 */
	class AudioDevice: public virtual device::Device {

		public:
			AudioDevice();

			/**
			 * The advance for all audio devices that actually does the 'work.'
			 * Fills the output buffer with updated samples based on the characteristics of the device,
			 * generally manipulating samples taken from other devices' buffers.
			 * Note that this method is pure virtual, and must be implemented on all audio devices
			 * @return A pointer to the buffer of samples of type ```sample_t```
			 */
			virtual sample_t * advance() = 0;

			/**
			 * Reads the buffer. If the device has not yet been advanced, it advances it
			 * Can be used to create "multiple outputs," where two devices read
			 * the buffer of another without needing to change the buffer
			 * @return A pointer to the buffer of samples of type ```sample_t```
			 */
			sample_t * read();

			/**
			 * Performs whatever cleanup is necessary. If not implemented, just resets the advanced bit.
			 */
			virtual void cleanup();

			/**
			 * Resize the buffer of all audio devices
			 * @param newSize New size of buffer in samples. Must be > 0
			 */
			void setBufferSize(int newSize);

			/**
			 * Return the buffer size
			 * @return Number of samples making up the audio buffer
			 */
			int getBufferSize();

			/**
			 * Change the sample rate of the device
			 * @param newRate New sample rate, in samples per second. Must be > 0
			 */
			void setSampleRate(int newRate);

			/**
			 * Return the sample rate of the device
			 * @return The sample rate, in samples per second.
			 */
			int getSampleRate();

			/**
			 * Copies ```numSamples``` samples from another buffer over to the
			 * devices own buffer
			 * @param otherBuffer A pointer to the other buffer to copy from
			 * @param numSamples The number of samples to copy over, usually the same
			 * as ```bufferSize```
			 */
			void copyToBuffer(sample_t * otherBuffer, int numSamples);

			/**
			 * Called at the end of any audio callback/buffer calculation. Resets the ```advanced```
			 * bit in all audio devices
			 */
			static void endOfBuffer();

			virtual ~AudioDevice();

		protected:

			/**
			 * Quickly set every sample in the buffer to 0
			 */
			void zeroBuffer();

			/**
			 * Quickly set every sample in the buffer to the given value
			 * @param sampVal The value to set all samples to
			 */
			void setBuffer(sample_t sampVal);

			/**
			 * Resizes the buffer of this AudioDevice
			 */
			void resizeBuffer();

			/**
			 * Resets the ```advanced``` bit to 0, called by ```endOfBuffer()``` at the end of
			 * a buffer calculation/callback
			 */
			void resetAdvanceBit();

			/**
			 * The output buffer of samples
			 */
			sample_t * buffer;

			/**
			 * An indicator showing whether or not the device has been advanced
			 */
			int advanced;

	};

	/**
	 * List of all audio devices. Used for buffer resizing and ```advanced``` bit resetting
	 */
	static std::list<AudioDevice *> audioDeviceList;
}
#endif //AUDIOUDEVICE_HPP_
