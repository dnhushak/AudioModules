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
#include "Parameter.h"

namespace device {
	enum devState_t{
		INACTIVE, ACTIVE
	};

	static int devIDCounter = 0;

	class Device{
		public:
			Device();

			virtual Device * clone(int);

			virtual void erase(int);

			virtual void alter(int paramNum, Parameter p);

			virtual int getNumParameters();

			/**
			 * Get the current device state
			 * @return Current device state, generally either ```INACTIVE``` (0), or ```ACTIVE``` (1)
			 */
			virtual int getState();

			virtual void setState(int newState);

			int getDevID();

			virtual ~Device();

		protected:
			int state;
			int numParameters;

		private:
			int devID;
	};

}

#endif /* DEVICE_h_ */
