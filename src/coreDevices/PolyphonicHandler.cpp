#include "PolyphonicHandler.h"

namespace device {
	
	PolyphonicHandler::PolyphonicHandler() {
		numVoices = 0;
		maxNumVoices = -1;
		upstream = 0;
	}
	
	void PolyphonicHandler::setMaxNumVoices(int newMaxNumVoices) {
		if (newMaxNumVoices == -1 || newMaxNumVoices > 0) {
			maxNumVoices = newMaxNumVoices;
		}
	}

	void PolyphonicHandler::setUpstream(
			Connectable<Device, Device> * newUpstream) {
		upstream = newUpstream;
	}

	void PolyphonicHandler::disconnectUpstream() {
		// Check if upstream still exists
		if (upstream != 0) {
			// TODO: Remove all voices from upstream device
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
					// Copy the current device tree (hehe)
					Device * NewTree = this->front->clone(2);

					// If there is an upstream device, attach the new device tree to upstream
					if (upstream != 0) {
						upstream->connectDevice(NewTree);
					}
				}
			}
		}
	}

	void PolyphonicHandler::cleanup() {
	}

	void PolyphonicHandler::deactivateVoice(int voiceNumber) {
		// Check if voiceNumber does exist in map
		if (voiceMap.count(voiceNumber)) {
		}
		// If it does, deactivate it
		// Decrement the voice counter
		numVoices--;
	}

	PolyphonicHandler::~PolyphonicHandler() {
	}

}
