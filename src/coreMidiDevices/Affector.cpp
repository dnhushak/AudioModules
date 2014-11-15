#include "Affector.h"

namespace midi {
	
	Affector::Affector() {
		learning = 0;
		listen.statusType = NOTEON;
		listen.data1 = -1;
		listen.data2 = 0;
		listen.channel = 0;
		filterByData1 = 0;
		intLo = 0;
		intHi = 127;
		floLo = 0;
		floHi = 127;
	}
	
	void Affector::affect(MIDIMessage * message) {
		// In learning mode
		if (learning) {
			listen.statusType = message->statusType;
			listen.data1 = message->data1;
			listen.data2 = message->data2;
			learning = 0;
		} else {
			if (message->statusType == listen.statusType) {
				// If
				if (filterByData1 && (listen.data1 == message->data1)) {
					// TODO: Send data2
				}
				else{
					// TODO: Send data1 and data2
				}
			}

		}
	}

	void Affector::learn() {
		learning = 1;
	}

	void Affector::setIntScale(int lo, int hi) {
		intLo = lo;
		intHi = hi;
	}

	void Affector::setFloatScale(float lo, float hi) {
		floLo = lo;
		floHi = hi;
	}

	void Affector::setTrigger(StatusType status) {
		if (status != CC) {
			listen.statusType = status;
		}
	}

	void Affector::setTrigger(StatusType status, char data1) {
		if (data1 >= 0 && data1 < 128) {
			listen.statusType = status;
			listen.data1 = 0;
		}
	}

	void Affector::activateData1Filter() {
		filterByData1 = 1;
	}

	void Affector::deactivateData1Filter() {
		filterByData1 = 0;
	}

	Affector::~Affector() {
	}

}
