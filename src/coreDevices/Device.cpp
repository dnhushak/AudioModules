#include "Device.h"

namespace device {
	
	Device::Device() {
		// State
		state = ACTIVE;
		// Device ID
		devID = devIDCounter;
		devIDCounter++;
		connectable = false;
		alterable = false;
	}

	Device * Device::clone(int) {
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

	void Device::setState(devState_t newState) {
		state = newState;
	}

	Device::~Device() {
		state = INACTIVE;
	}

}
