#include "RGBLED.h"

namespace ArduinoUI {
	RGBLED::RGBLED(int redPin, int grnPin, int bluPin) {
		redLED = new ArduinoUI::LED(redPin);
		grnLED = new ArduinoUI::LED(grnPin);
		bluLED = new ArduinoUI::LED(bluPin);
		RGBLEDstate = OFF;
		// Timing variables
		currentTime = millis();
		lastTime = currentTime;
		blinkTime = 1000;
	}
	void RGBLED::setColor(unsigned char newRedIntensity,
			unsigned char newGrnIntensity, unsigned char newBluIntensity) {
		redLED->setIntensity(newRedIntensity);
		grnLED->setIntensity(newGrnIntensity);
		bluLED->setIntensity(newBluIntensity);
	}
	void RGBLED::begin() {
		redLED->begin();
		grnLED->begin();
		bluLED->begin();
	}
	void RGBLED::on() {
		redLED->on();
		grnLED->on();
		bluLED->on();
		RGBLED = ON;
	}

	void RGBLED::off() {
		redLED->on();
		grnLED->on();
		bluLED->on();
		RGBLED = OFF;
	}

	// These are intended to be called in the loop
	void RGBLED::blink(unsigned long newBlinkTime) {
		blinkTime = newBlinkTime;
		currentTime = millis();
		if (currentTime - lastTime > blinkTime) {
			toggle();
			lastTime = currentTime;
		}
	}

	void RGBLED::blink() {
		currentTime = millis();
		if (currentTime - lastTime > blinkTime) {
			toggle();
			lastTime = currentTime;
		}
	}

	void RGBLED::toggle() {
		// If LEDstate is on, then turn off
		if (RGBLED) {
			off();
		}
		// Else turn on
		else {
			on();
		}
	}
}
