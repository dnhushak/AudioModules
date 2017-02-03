#include "Device.h"

namespace device {
	
	Device::Device() {
		// State
		state = ACTIVE;
		// Device ID
		devID = devIDCounter;
		devIDCounter++;
		numParameters = 0;
	}

	Device * Device::clone(int) {
		Device * newDevice = new Device();
		newDevice->state = this->getState();
		return newDevice;
	}

	void Device::erase(int){
		delete this;
		return;
	}

	int Device::getDevID() {
		return devID;
	}

	void Device::alter(int paramNum, Parameter p){
		return;
	}

	int Device::getNumParameters(){
		return numParameters;
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
