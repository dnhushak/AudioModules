//Audio Effect Class
#pragma once
#include "AudioDevice.hpp"
#include <forward_list>
#include <iostream>

namespace chip {

	/** An audio effect. Has a list of AudioDevices that can be manipulated in some way
	 * User has to add devices to device list using the single add, forward_list add, or repoint methods
	 * User has an option to set the maximum number of devices in the list, which will affect additions.
	 * Default upon adding an AudioDevice to a full AudioEffect list is to ignore if at maximum
	 *
	 * User must implement the advance() method inherited from the AudioDevice class.
	 * Best to make use of the std::forward_list capabilities to dynamically
	 * iterate through each AudioDevice in the list.
	 *
	 * Simple examples of an AudioEffect would be:
	 * -Mixer: sums all buffers in the device list
	 * -Gain: scales value of a buffer
	 * -Filter: implements some FIR or otherwise filter method to stream
	 */
	class AudioEffect: public AudioDevice {
		public:
			AudioEffect();

			// Resize the buffer of the object, as well as all of its downstream connected objects
			// Overriding AudioDevice::resizeBuffer()
			virtual void resizeBuffer(int);

			virtual void cleanup();

			// Adds audio object to mixer list
			void addAudioDevice(AudioDevice *);

			// Adds a forward_list of audio objects to mixer list
			//void addAudioDevices(std::forward_list<AudioDevice *> *);

			// Changes the device list by pointing to a different one
			void setAudioDeviceList(std::forward_list<AudioDevice *> *);

			// Removes audio object from mixer list based on audio object reference
			void removeAudioDevice(AudioDevice *);

			// Removes audio object from mixer based on forward_list index
//			void removeAudioDevice(int);

			// Remove all objects from the mixer
			void removeAllAudioDevices();

			// Returns the number of objects in this mixer
			int getNumAudioDevices();
		protected:

			// List of Audio items to mix together
			std::forward_list<AudioDevice*> * audioDeviceList;
			// List iterator
			std::forward_list<AudioDevice*>::iterator audIter;

			// Maximum number of audio devices allowable in effect (-1 -> no maximum)
			int maxNumAudioDevices;

			// Actual number of audio devices in effect
			int numAudioDevices;

	};
}
