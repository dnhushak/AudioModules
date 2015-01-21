#ifndef AFFECTOR_h_
#define AFFECTOR_h_
#include "MIDIDevice.h"
#include "ConnectableDevice.h"

namespace midi {
	
	class Affector: public midi::MIDIDevice, public device::ConnectableDevice<
			device::Device, Affector> {
		public:
			Affector();

			/**
			 * Takes in MIDI input, compares it to the ```listen``` message
			 * stored within the affector, and if it matches, passes the data value
			 * to its connected devices' ```parameter``` function.
			 *
			 * If the ```learn``` bit is set to 1, the ```Affector``` will take the first MIDI message input
			 * into it and set it as the ``listen``` message type.
			 * @param MIDIMessage, the message received by the router
			 */
			void affect(MIDIMessage *);

			/**
			 * Sets the learn bit to 1, making the ```Affector``` change its ```listen``` message type to
			 * that of the first MIDIMessage it receives.
			 */
			void learn();

			/**
			 * Sets the scaling factor for MIDI input to the integer input of the ```parameter``` function
			 * @param lo
			 * @param hi
			 */
			void setIntScale(int lo, int hi);

			/**
			 * Sets the scaling factor for MIDI input to the float input of the ```parameter``` function
			 * @param lo
			 * @param hi
			 */
			void setFloatScale(float lo, float hi);

			/**
			 * Manually sets what status bit to listen for
			 * If status is of type ```CC```, setTrigger will do nothing, as a specification
			 * of controller number is relevant for a ```CC``` type to be used
			 * @param status The status type to listen for
			 */
			void setTrigger(StatusType status);

			/**
			 * Manually sets what status and data bit (for cc) to listen for
			 * @param status The status type to listen for
			 * @param data1 The first data bit to listen for (for cc type messages)
			 */
			void setTrigger(StatusType status, unsigned char data1);

			/**
			 * Sets the router to route by ```data1```, instead of passing it. Generally used for
			 * ```CC``` type messages, where the first byte is the CC number and the second is actually the data
			 */
			void activateData1Filter();

			/**
			 * Sets the router to pass ```data1``` and ```data2``` from messages, routing only by the status type
			 */
			void deactivateData1Filter();

			virtual ~Affector();
		private:
			int learning;
			int filterByData1;
			int intLo, intHi;
			float floLo, floHi;
			MIDIMessage listen;
	};

}

#endif /* AFFECTOR_h_ */
