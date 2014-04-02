#include "LED.h"
namespace UI {

	LED::LED(int newPin) {
		pin = newPin;
		intensity = 255;
	}

	void LED::begin() {
		pinMode(pin, OUTPUT);
	}

	void LED::setIntensity(unsigned char newIntensity) {
		intensity = newIntensity;
	}
	unsigned char LED::getIntensity() {
		return intensity;
	}

	void LED::on() {
		analogWrite(pin, 255 - intensity);
	}

	void LED::off() {
		analogWrite(pin, 255);
	}

}
