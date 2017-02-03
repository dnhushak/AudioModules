#include "Device.h"

namespace device {

	Device::Device(){
		// State
		state = ACTIVE;
		// Device ID
		devID = devIDCounter;
		devIDCounter++;
		numParameters = 1;
	}

	Device * Device::clone(int){
		Device * newDevice = new Device();
		newDevice->state = this->getState();
		return newDevice;
	}

	void Device::erase(int){
		delete this;
		return;
	}

	int Device::getDevID(){
		return devID;
	}

	void Device::alter(int paramNum, Parameter p){
		switch (paramNum){
			case 0:
				this->setState(p.getParam().i);
				break;
		}
		return;
	}

	int Device::getNumParameters(){
		return numParameters;
	}

	int Device::getState(){
		return state;
	}

	void Device::setState(int newState){
		state = newState;
	}

	Device::~Device(){
		state = INACTIVE;
	}

}
