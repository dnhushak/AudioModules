#include "LED.h"
namespace ArduinoUI {

	LED::LED(int newPin) {
		pin = newPin;
		intensity = 255;
		LEDstate = OFF;
		currentTime = millis();
		lastTime = currentTime;
		blinkTime = 1000;
	}

	void LED::begin() {
		pinMode(pin, OUTPUT);
		off();
	}

	void LED::setIntensity(unsigned char newIntensity) {
		intensity = newIntensity;
	}
	unsigned char LED::getIntensity() {
		return intensity;
	}

	void LED::on() {
		// Prevents successive writes to an LED pin in a loop
		if (LEDstate == OFF) {
			analogWrite(pin, 255 - intensity);
			LEDstate = ON;
		}
	}

	void LED::off() {
		// Prevents successive writes to an LED pin in a loop
		if (LEDstate == ON) {
			analogWrite(pin, 255);
			LEDstate = OFF;
		}
	}

	void LED::toggle() {
		// If state is on, then turn off
		if (LEDstate) {
			off();
		}
		// Else turn on
		else {
			on();
		}
	}

	// These are intended to be called in the loop
	void LED::blink(unsigned long newBlinkTime) {
		blinkTime = newBlinkTime;
		currentTime = millis();
		if (currentTime - lastTime > blinkTime) {
			toggle();
			lastTime = currentTime;
		}
	}

	void LED::blink() {
		currentTime = millis();
		if (currentTime - lastTime > blinkTime) {
			toggle();
			lastTime = currentTime;
		}
	}
}
