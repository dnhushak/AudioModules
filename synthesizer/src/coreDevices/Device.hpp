#ifndef DEVICE_HPP_
#define DEVICE_HPP_

namespace synth {
	enum devState_t {
		ACTIVE, INACTIVE,
	};
	
	class Device {
		public:
			Device();

			// Return the state
			virtual devState_t getState();

			virtual ~Device();
		protected:
			devState_t state;
	};

}

#endif /* DEVICE_HPP_ */
