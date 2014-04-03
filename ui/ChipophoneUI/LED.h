#ifndef LED_H_
#define LED_H_

#include "Arduino.h"

namespace ArduinoUI {
	enum LEDState{
		OFF, ON
	};

	class LED {
		public:
			LED(int newPin);
			void begin();
			void setIntensity(unsigned char intensity);
			unsigned char getIntensity();
			void on();
			void off();
			void toggle();
			void blink(unsigned long blinkTime);
			void blink();

		private:
			int pin;
			int LEDstate;
			unsigned char intensity;

			// Blink timing variables
			unsigned long currentTime;
			unsigned long lastTime;
			unsigned long blinkTime;

	};

}
#endif //LED_H_
