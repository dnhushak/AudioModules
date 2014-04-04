#include "ChipScreenController.h"

namespace chip {
	
	ChipScreenController::ChipScreenController(int cs, int dc) {
		chipScreen = new Adafruit_ST7735(cs, dc, 0);
	}
	
	void ChipScreenController::begin() {
		// Initialize screen
		chipScreen->initR(INITR_BLACKTAB);
		// Fill the screen black
		chipScreen->fillScreen(ST7735_BLACK);
		chipScreen->setCursor(0, 0);
		chipScreen->setTextColor(ST7735_WHITE);
		chipScreen->setTextWrap(true);
	}

	ChipScreenController::~ChipScreenController() {
		delete chipScreen;
	}

}
