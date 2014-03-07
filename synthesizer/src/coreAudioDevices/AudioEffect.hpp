//Audio Effect Class
#pragma once
#include "AudioDevice.hpp"
#include <vector>
#include <iostream>

namespace chip {
	class AudioEffect: public AudioDevice {
		public:
			// Resize the buffer of the object, as well as all of its downstream connected objects
			// Overriding AudioDevice::resizeBuffer()
			void resizeBuffer(int);

			// Adds audio object to mixer list
			void addObject(AudioDevice *);

			// Adds a vector of audio objects to mixer list
			void addObjects(std::vector<AudioDevice *> *);

			// Changes the device list by pointing to a different one
			void setDeviceList(std::vector<AudioDevice *> *);

			// Removes audio object from mixer list based on audio object reference
			void removeObject(AudioDevice *);

			// Removes audio object from mixer based on vector index
			void removeObject(int);

			// Remove all objects from the mixer
			void removeAllObjects();

			// Returns the number of objects in this mixer
			int getNumObjects();
		protected:

			// List of Audio items to mix together
			std::vector<AudioDevice*> * audioDeviceList;

	};
}
