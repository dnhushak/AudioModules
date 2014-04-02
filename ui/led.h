#ifndef LED_H_
#define LED_H_

enum color {
	RED, YEL, GRN, BLU, WHT
};

typedef struct rgbled {
		int red;
		int green;
		int blue;
} rgbled_t;

#endif /* LED_H_ */
