#include "led.h"

void setLEDColor(rgbled_t rgbPin, int color) {

	switch (color) {
		case RED:
			turnOn(rgbPin.red);
			turnOff(rgbPin.green);
			turnOff(rgbPin.blue);
			break;

		case YEL:
			turnOn(rgbPin.red);
			turnOn(rgbPin.green);
			turnOff(rgbPin.blue);
			break;

		case GRN:
			turnOff(rgbPin.red);
			turnOn(rgbPin.green);
			turnOff(rgbPin.blue);
			break;

		case BLU:
			turnOff(rgbPin.red);
			turnOff(rgbPin.green);
			turnOn(rgbPin.blue);
			break;

		case WHT:
			turnOn(rgbPin.red);
			turnOn(rgbPin.green);
			turnOn(rgbPin.blue);
			break;

		default:
			turnOff(rgbPin.red);
			turnOff(rgbPin.green);
			turnOff(rgbPin.blue);
	}
}
