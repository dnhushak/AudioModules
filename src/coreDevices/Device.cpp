#include "Device.hpp"

namespace device {
	
	Device::Device() {
		initParameters(0);
		// State
		state = ACTIVE;
		// Device ID
		devID = devIDCounter;
		devIDCounter++;

	}

	int Device::getDevID() {
		return devID;
	}

	devState_t Device::getState() {
		return state;
	}

	Device::~Device() {
		state = INACTIVE;
		free(parameter);
	}

}
