#include "Encoder.h"

namespace ArduinoUI {

	/*
	 * Encoder Wiring:
	 *
	 * 		Encoder
	 * Pin A  Common  Pin B
	 *    o		o		o
	 *    |		|		|
	 *    |		|		|
	 *  o_|		V		|_o
	 * Pin #   GND		Pin #
	 * 		Arduino
	 */
	Encoder::Encoder(int initPinA, int initPinB) {
		pinA = initPinA;
		pinB = initPinB;
		setCurrentVal(0);
		setMinVal(0);
		setMaxVal(127);
	}

	void Encoder::begin() {
		// Set mode to input
		pinMode(pinA, INPUT);
		pinMode(pinB, INPUT);

		// Turn on internal pullup resistors
		digitalWrite(pinA, HIGH);
		digitalWrite(pinB, HIGH);

		// Initialize the stored states of the pins
		aLastState = digitalRead(pinA);
		bLastState = digitalRead(pinB);
	}

	void Encoder::setMinVal(int newMinVal) {
		// Set the new minimum value
		minVal = newMinVal;

		normalizeCurrentVal();
	}

	void Encoder::setMaxVal(int newMaxVal) {
		// Set the new maximum value
		maxVal = newMaxVal;
		normalizeCurrentVal();
	}

	void Encoder::setCurrentVal(int newCurrentVal) {
		currentVal = newCurrentVal;
		normalizeCurrentVal();
	}

	int Encoder::getCurrentVal() {
		return currentVal;
	}

	int Encoder::hasChanged(){
		// XOR for a not equal check, represents a change since the last time button was checked
		int notEqual = currentVal ^ lastCheckedVal;
		if(notEqual){
			// If they are not equal, update the last state to current state so the next immediate check returns false
			lastCheckedVal = currentVal;
			// Sets to 1 to represent not equal
			notEqual = 1;
		}
		return notEqual;
	}

	void Encoder::poll() {
		/*
		 * Here is a diagram of the two waveforms in an encoder rotation:
		 *
		 * CW  -->
		 * CCW <--
		 *
		 * A waveform
		 * __|ררר|___|ררר|___
		 * |ררר|___|ררר|___
		 * B waveform
		 *
		 */

		aPolledState = digitalRead(pinA);
		bPolledState = digitalRead(pinB);
		// Edge on the A pin
		if (aPolledState != aLastState) {
			bLastState ^ aLastState ? currentVal++ : currentVal--;
			aLastState = aPolledState;
			normalizeCurrentVal();
		}

		// Edge on the B pin
		if (bPolledState != bLastState) {
			bLastState = bPolledState;
		}

	}

	void Encoder::normalizeCurrentVal() {
		if (currentVal > maxVal) {
			currentVal = maxVal;
		} else if (currentVal < minVal) {
			currentVal = minVal;
		}
	}
}
