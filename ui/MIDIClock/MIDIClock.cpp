#include "MIDIClock.h"

namespace synth {
	
	MIDIClock::MIDIClock() {
		timingClock.statusType = SYSTEM;
		timingClock.channel = TIMECLOCK;
		initTimer();
	}
	
	void MIDIClock::initTimer(){
		// Disable OCR pin
		TCCR1A |= 0b00001100;	//WGM:CTC, COM:OC2 disconnected,pre_scaler = 1
		TCCR1A ^= 0b00001100;
		TIMSK1 |= 0b10000000;	//Enabling O.C. Interrupt for Timer2
	}

	static void tick(void * clock){

	}

	MIDIClock::~MIDIClock() {

	}

}
