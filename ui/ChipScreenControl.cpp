#include "ChipScreenControl.h"

namespace chip {

	ChipScreenControl::ChipScreenControl(ScreenPins * pinout) {
		// Grab pinout from struct
		csPin = pinout->cs;
		dcPin = pinout->dc;
		rstPin = pinout->rst;
		bgColor = ST7735_RED;
		textColor = ST7735_BLACK;
		tft = new Adafruit_ST7735(csPin, dcPin, rstPin);
	}

	void ChipScreenControl::begin() {
		// Initialize the screen
		tft->initR(INITR_BLACKTAB);

		// Rotate our coord system since we're sideways
		tft->setRotation(1);
		// Fill screen black to begin
		welcome();
	}

	void ChipScreenControl::welcome() {
		tft->setTextWrap(false);
		tft->fillScreen(bgColor);
		tft->setTextColor(textColor);
		tft->setTextSize(2);
		tft->setCursor(30, 10);
		tft->println("Welcome");
		tft->setCursor(40, 40);
		tft->println("To The");
		tft->setCursor(20, 70);
		tft->println("Chipophone!");
		// Drawing the 1/8th notes!
		tft->fillCircle(70,115,4,ST7735_CYAN);
		tft->fillCircle(90,115,4,ST7735_CYAN);
		tft->fillRect(72,93,3,20,ST7735_CYAN);
		tft->fillRect(92,93,3,20,ST7735_CYAN);
		tft->fillRect(72,93,20,3,ST7735_CYAN);
	}
	void ChipScreenControl::writeTextTop(char * text) {
		tft->setTextWrap(false);
		tft->fillRect(10, 10, 150, 24, bgColor);
		tft->setCursor(10, 10);
		tft->setTextColor(textColor);
		tft->setTextSize(3);
		tft->println(text);
	}

	void ChipScreenControl::writeTextMid(char * text) {
		tft->setTextWrap(false);
		tft->fillRect(10, 35, 150, 24, bgColor);
		tft->setCursor(10, 35);
		tft->setTextColor(textColor);
		tft->setTextSize(3);
		tft->println(text);
	}

	void ChipScreenControl::writeTextBot(char * text) {
		tft->setTextWrap(false);
		tft->fillRect(50, 70, 110, 24, bgColor);
		tft->setCursor(50, 70);
		tft->setTextColor(textColor);
		tft->setTextSize(3);
		tft->println(text);
	}

	void ChipScreenControl::setRed() {
		bgColor = ST7735_RED;
		textColor = ST7735_BLACK;
		tft->fillScreen(bgColor);
	}
	void ChipScreenControl::setYel() {
		bgColor = ST7735_YELLOW;
		textColor = ST7735_BLACK;
		tft->fillScreen(bgColor);
	}
	void ChipScreenControl::setGrn() {
		bgColor = ST7735_GREEN;
		textColor = ST7735_BLACK;
		tft->fillScreen(bgColor);
	}
	void ChipScreenControl::setBlu() {
		bgColor = ST7735_BLUE;
		textColor = ST7735_WHITE;
		tft->fillScreen(bgColor);
	}
	void ChipScreenControl::setWht() {
		bgColor = ST7735_WHITE;
		textColor = ST7735_BLACK;
		tft->fillScreen(bgColor);
	}
	ChipScreenControl::~ChipScreenControl() {
		delete tft;
	}

}
