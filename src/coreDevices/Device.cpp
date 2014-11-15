#include "Device.h"

namespace device {
	
	Device::Device() {
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
	}

}
