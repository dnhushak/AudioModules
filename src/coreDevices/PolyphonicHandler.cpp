#include "PolyphonicHandler.h"

namespace device {
	
	PolyphonicHandler::PolyphonicHandler() {
		numVoices = 0;
		maxNumVoices = -1;
		upstream = 0;

		// Can only have 1 device attached to it,
		// that device and any subsequently connected
		// devices will be the base "voice" for any polyphony
		setMaxNumDevices(1);
	}
	
	void PolyphonicHandler::setMaxNumVoices(int newMaxNumVoices) {
		if (newMaxNumVoices == -1 || newMaxNumVoices > 0) {
			maxNumVoices = newMaxNumVoices;
		}
	}

	void PolyphonicHandler::setUpstream(
			Connectable<Device, Device> * newUpstream) {
		upstream = newUpstream;
		// Start at the beginning of the voice map
		voiceIter = voiceMap.begin();
		// Iterate over the whole map
		for (voiceIter=voiceMap.begin();voiceIter != voiceMap.end();voiceIter++) {
			// Add all of the devices in the voice map from the upstream device
			upstream->connectDevice(voiceIter->second);
		}

	}

	void PolyphonicHandler::disconnectUpstream() {
		// Check if upstream still exists
		if (upstream != 0) {
			// Iterate over the whole map
			for (voiceIter=voiceMap.begin();voiceIter != voiceMap.end();voiceIter++) {
				// Get rid of all of the devices in the voice map from the upstream device
				upstream->disconnectDevice(voiceIter->second);
			}
		}
		upstream = 0;
	}

	void PolyphonicHandler::activateVoice(int voiceNumber, Parameter param) {
		// Check if voiceNumber already exists
		// If it does, update it with new parameter
		if (voiceMap.count(voiceNumber)) {
			//TODO: Update current voice with new parameter
		}

		// If it doesn't...
		else {
			// Check to see if we have voices available
			if (maxNumVoices == -1 || numVoices < maxNumVoices) {
				// Increment the voice counter
				numVoices++;

				if (!isEmpty()) {
					// Copy the current device tree
					Device * newTree =  this->front()->clone(WHOLETREE);

					// Add the new tree to the voiceMap
					voiceMap.insert(std::pair<int,Device *>(voiceNumber, newTree));

					// If there is an upstream device, attach the new device tree to upstream
					if (upstream != 0) {
						upstream->connectDevice(newTree);
					}
				}
			}
		}
	}

	void PolyphonicHandler::cleanup() {
		voiceIter = voiceMap.begin();
		// Iterate over the whole map
		while (voiceIter != voiceMap.end()) {
			// Check for inactive voices
			if ( voiceIter->second->getState() == INACTIVE) {
				if (upstream != 0) {
					// Disconnect the device from upstream
					upstream->disconnectDevice(voiceIter->second);
				}
				voiceMap.erase(voiceIter);
			}
		}
	}

	void PolyphonicHandler::deactivateVoice(int voiceNumber) {
		// Check if voiceNumber does exist in map
		if (voiceMap.count(voiceNumber)) {
		}
		// If it does, deactivate it
		voiceMap[voiceNumber]->setState(INACTIVE);

		// Decrement the voice counter
		numVoices--;
	}

	PolyphonicHandler::~PolyphonicHandler() {
		voiceIter = voiceMap.begin();
		while (voiceIter != voiceMap.end()) {
			// Get rid of all of the devices in the voice map from the upstream device

			voiceIter++;
		}
	}
}
