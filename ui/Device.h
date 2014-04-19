#ifndef DEVICE_HPP_
#define DEVICE_HPP_
#include <stdio.h>
#include <cstdlib>

namespace synth {
	enum devState_t {
		INACTIVE, ACTIVE
	};

	static int devIDCounter;
	class Device {
		public:
			Device();

			// Return the state
			virtual devState_t getState();

			int getDevID();

			virtual ~Device();

		protected:
			int devID;
			devState_t state;
	};

}

#endif /* DEVICE_HPP_ */
