#include "Arduino.h"
#include "LED.h"

namespace ArduinoUI {
	class RGBLED {
		public:
			RGBLED(int redPin, int grnPin, int bluPin);
			void setColor(unsigned char newRedIntensity,
					unsigned char newGrnIntensity,
					unsigned char newBluIntensity);
			void begin();
			void on();
			void off();
			void toggle();
			void blink();
			void blink(unsigned long newBlinkTime);
			~RGBLED();
		private:
			ArduinoUI::LED * redLED;
			ArduinoUI::LED * grnLED;
			ArduinoUI::LED * bluLED;
			int RGBLEDstate;

			// Blink timing variables
			unsigned long currentTime;
			unsigned long lastTime;
			unsigned long blinkTime;
	};

}
