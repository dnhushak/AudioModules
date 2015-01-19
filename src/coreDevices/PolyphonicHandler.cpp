#include "PolyphonicHandler.h"

namespace device {
	
	PolyphonicHandler::PolyphonicHandler() {
		numTimbres = 1;
		curTimbre = 0;
		numVoices = 0;
		maxNumVoices = -1;
		upstream = 0;
	}
	
	void PolyphonicHandler::setMaxNumVoices(int newMaxNumVoices) {
		if (newMaxNumVoices == -1 || newMaxNumVoices > 0) {
			maxNumVoices = newMaxNumVoices;
		}
	}

	void PolyphonicHandler::setNumTimbres(int newNumTimbres) {
		if (newNumTimbres > 0) {
			numTimbres = newNumTimbres;
		}
	}

	void PolyphonicHandler::setUpstream(
			ConnectableDevice<Device> * newUpstream) {
		upstream = newUpstream;
	}

	void PolyphonicHandler::disconnectUpstream() {
		// Check if upstream still exists
		if (upstream != 0) {
			// Remove all voices from upstream device
		}
		upstream = 0;
	}

	void PolyphonicHandler::activateVoice(int voiceNumber) {
		// Check if voiceNumber already exists in map
		// If it does, update it with new parameter
		if (voiceMap.count(voiceNumber)) {

		}
		// If it doesn't...
		else {
			// Check to see if we have voices available
			if (maxNumVoices == -1 || numVoices < maxNumVoices) {
				// Increment the voice counter
				numVoices++;

				// Copy the current timbre tree (hehe)
				Device * NewTree = (this->copyConnectedTree())->getDeviceByLoc(curTimbre);
				//TODO: Figure out the best way to do this without copying all timbre trees (maybe clone is the best option?)

				// Increment the timbre counter
				curTimbre++;
				// Make sure the timbre counter is within bounds
				curTimbre = (numTimbres - 1) % curTimbre;

				// If there is an upstream device, attach the new timbre tree to upstream
				if (upstream != 0) {
					upstream->addDevice(NewTree);
				}
			}
		}
	}

	void PolyphonicHandler::cleanup() {
	}

	void PolyphonicHandler::deactivateVoice(int voiceNumber) {
		// Check if voiceNumber does exist in map
		// If it does, deactivate it
		// Decrement the voice counter
		numVoices--;
	}

	PolyphonicHandler::~PolyphonicHandler() {
	}

}
