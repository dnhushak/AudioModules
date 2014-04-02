#ifndef LED_H_
#define LED_H_

#include "Arduino.h"

namespace UI {

	class LED {
		public:
			LED(int newPin);
			void begin();
			void setIntensity(unsigned char intensity);
			unsigned char getIntensity();
			void on();
			void off();

		private:
			int pin;
			unsigned char intensity;

	};

}
#endif //LED_H_
