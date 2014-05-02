#ifndef MIDICLOCK_H_
#define MIDICLOCK_H_
#include "MIDIDevice.h"
#include "Arduino.h"
#include <avr/io.h>
#include <avr/interrupt.h>
extern TimerOne Timer1;

namespace synth {
	
	class MIDIClock : public MIDIDevice {
		public:
			MIDIClock();
			void initTimer();
			virtual ~MIDIClock();
		private:
			MIDIMessage timingClock;
	};

}

#endif /* MIDICLOCK_H_ */
