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
	ArduinoUI::Encoder::Encoder(int initPinA, int initPinB) {
		pinA = initPinA;
		pinB = initPinB;
		setCurrentVal(0);
		setMinVal(0);
		setMaxVal(127);
	}

	void ArduinoUI::Encoder::begin() {
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

	void ArduinoUI::Encoder::setMinVal(int newMinVal) {
		// Set the new minimum value
		minVal = newMinVal;

		normalizeCurrentVal();
	}

	void ArduinoUI::Encoder::setMaxVal(int newMaxVal) {
		// Set the new maximum value
		maxVal = newMaxVal;
		normalizeCurrentVal();
	}

	void ArduinoUI::Encoder::setCurrentVal(int newCurrentVal) {
		currentVal = newCurrentVal;
		normalizeCurrentVal();
	}

	int ArduinoUI::Encoder::getCurrentVal() {
		return currentVal;
	}

	void ArduinoUI::Encoder::poll() {
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
