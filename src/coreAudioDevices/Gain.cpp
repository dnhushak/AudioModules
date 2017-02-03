#include "Gain.h"

namespace audio {

	Gain::Gain(){
		setMaxNumDevices(1);
		// Default gain to 0 db
		setGain(0);
	}

	Gain * Gain::clone(int cloneType){
		if (cloneType != 0){
			return (Gain *) Connectable::clone(cloneType);
		}
		// Create new device
		Gain * newDevice = new Gain();
		// Set all member variables

		newDevice->state = this->state;
		newDevice->gain = this->gain;

		return newDevice;
	}

	void Gain::alter(int paramNum, Parameter p){
		Connectable::alter(paramNum,p);
		Device::alter(paramNum,p);
		switch (paramNum){
			case 1:
				//Gain
				setGain(p.getParam().d);
				break;
		}
	}

	sample_t * Gain::advance(){
		if (isEmpty()){
			zeroBuffer();
		}
		else{
			copyToBuffer(front()->read(), bufferSize);
			for (int i = 0; i < bufferSize; i++){
				buffer[i] *= gain;
			}
		}
		return buffer;
	}

	void Gain::setGain(float volume){
		// Volume is coming in dbs
		// 0 dbs => gain of 1
		gain = dbToRatio(volume);
	}
}
