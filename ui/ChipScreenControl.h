#ifndef CHIPSCREENCONTROL_H_
#define CHIPSCREENCONTROL_H_
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library
#include <SPI.h>

namespace chip {
	typedef struct ScreenPins {
			int cs;
			int dc;
			int rst;
	} ScreenPins;

	class ChipScreenControl {
		public:
			ChipScreenControl(ScreenPins * pinout);
			void begin();
			void writeTextTop(char * text);
			void writeTextMid(char * text);
			void writeTextBot(char * text);
			void welcome();
			void setRed();
			void setYel();
			void setGrn();
			void setBlu();
			void setWht();
			virtual ~ChipScreenControl();

		private:
			uint8_t csPin;
			uint8_t dcPin;
			uint8_t rstPin;
			Adafruit_ST7735 * tft;
			uint16_t bgColor;
			uint16_t textColor;
	};

}

#endif /* CHIPSCREENCONTROL_H_ */
