#include "extra.h"
#include "pinout.h"

void demoSequence() {
	int numLEDs = 15;
	int LEDs[] = { redSelectorLED, yelSelectorLED, grnSelectorLED,
			bluSelectorLED, whtSelectorLED, arpeggioRedLED, arpeggioGrnLED,
			arpeggioBluLED, glissRedLED, glissGrnLED, glissBluLED, playLED,
			pausLED, stopLED, recdLED };

	int j;
	int i;

	// Flash all the LEDs 5 times
	for (j = 0; j < 4; j++) {
		for (i = 0; i < numLEDs; i++) {
			turnOn(LEDs[i]);
		}
		delay(150);
		for (i = 0; i < numLEDs; i++) {
			turnOff(LEDs[i]);
		}
		delay(150);
	}

	// Flash each LED individually in sequence
	for (i = 0; i < numLEDs; i++) {
		turnOn(LEDs[i]);
		delay(100);
		turnOff(LEDs[i]);
	}

}

void mushroom() {
	byte message = (NOTEON << 4);
	byte data1;
	int i;
	for (i = 0; i < 26; i++) {
		switch (i) {
			case 0:
				data1 = 60;
				break;
			case 1:
				data1 = 53;
				break;
			case 2:
				data1 = 60;
				break;
			case 3:
				data1 = 64;
				break;
			case 4:
				data1 = 67;
				break;
			case 5:
				data1 = 72;
				break;
			case 6:
				data1 = 67;
				break;
			case 7:
				data1 = 56;
				break;
			case 8:
				data1 = 60;
				break;
			case 9:
				data1 = 63;
				break;
			case 10:
				data1 = 68;
				break;
			case 11:
				data1 = 63;
				break;
			case 12:
				data1 = 68;
				break;
			case 13:
				data1 = 72;
				break;
			case 14:
				data1 = 75;
				break;
			case 15:
				data1 = 80;
				break;
			case 16:
				data1 = 75;
				break;
			case 17:
				data1 = 58;
				break;
			case 18:
				data1 = 62;
				break;
			case 19:
				data1 = 65;
				break;
			case 20:
				data1 = 70;
				break;
			case 21:
				data1 = 65;
				break;
			case 22:
				data1 = 70;
				break;
			case 23:
				data1 = 74;
				break;
			case 24:
				data1 = 77;
				break;
			case 25:
				data1 = 82;
				break;
		}
		sendMidi(message, data1 + 12, 127);
		delay(35);
		sendMidi(message, data1 + 12, 0);

	}
}
