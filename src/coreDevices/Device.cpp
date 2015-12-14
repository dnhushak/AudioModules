#include "Device.h"

namespace device {
	
	Device::Device() {
		// State
		state = ACTIVE;
		// Device ID
		devID = devIDCounter;
		devIDCounter++;
	}

	Device * Device::clone() {
		Device * newDevice = new Device();
		newDevice->state = this->getState();
		return newDevice;
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
