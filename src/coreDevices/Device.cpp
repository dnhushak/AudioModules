#include "Device.h"

namespace device {
	
	Device::Device() {
		// State
		state = ACTIVE;
		// Device ID
		devID = devIDCounter;
		devIDCounter++;
//		parameterList = new std::List<Parameter>();

	}

	int Device::getDevID() {
		return devID;
	}

	devState_t Device::getState() {
		return state;
	}

	Device * Device::clone() {
		Device * cloned = new Device();
		cloned->state = state;
		return cloned;
	}

	Device::~Device() {
		state = INACTIVE;
	}

}
