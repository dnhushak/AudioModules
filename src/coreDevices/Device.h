// Generic Device class
#ifndef DEVICE_h_
#define DEVICE_h_

// For std::cout messages (debugging and message printing)
#include <iostream>
// For malloc, realloc
#include <cstdlib>
// For bind
#include <functional>
// For list
#include <list>

namespace device {
	enum devState_t {
		INACTIVE, ACTIVE
	};
	
	static int devIDCounter = 0;

	class Device {
		public:
			Device();

			/**
			 * Get the current device state
			 * @return Current device state, generally either ```INACTIVE``` (0), or ```ACTIVE``` (1)
			 */
			virtual devState_t getState();

			int getDevID();

			virtual ~Device();

		protected:

			devState_t state;

		private:

			int devID;
	};

}

#endif /* DEVICE_h_ */
