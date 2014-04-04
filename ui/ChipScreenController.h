#ifndef CHIPSCREENCONTROLLER_H_
#define CHIPSCREENCONTROLLER_H_
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library
#include <SPI.h>

namespace chip {
	
	class ChipScreenController {
		public:
			ChipScreenController(int cs, int dc);
			void begin();
			virtual ~ChipScreenController();

		private:
			Adafruit_ST7735 * chipScreen;
	};

}

#endif /* CHIPSCREENCONTROLLER_H_ */
