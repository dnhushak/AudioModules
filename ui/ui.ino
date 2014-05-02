#include "Arduino.h"
#include <Keypad.h>
#include <HardwareSerial.h>
#include "Button.h"
#include "LED.h"
#include "RGBLED.h"
#include "ArduinoMIDIHandler.h"
#include "Encoder.h"
#include "ChipSongboxControl.h"
#include "ChipModuleControl.h"
#include "ChipNumberPad.h"
#include "ChipScreenControl.h"
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library
#include <SPI.h>

/******************************************************************************/
/* Pinouts - Make any changes to pin assignments below                        */
/******************************************************************************/

/******************************************************************************/
/* Channel Selection Buttons and LEDs                                         */
/******************************************************************************/
const int REDBUTTON = 22;
const int YELBUTTON = 23;
const int GRNBUTTON = 24;
const int BLUBUTTON = 25;
const int WHTBUTTON = 26;

const int REDLED = 2;
const int YELLED = 3;
const int GRNLED = 4;
const int BLULED = 5;
const int WHTLED = 6;

/******************************************************************************/
/* Channel Edit Buttons, Encoders and LEDs                                    */
/******************************************************************************/

// Arpgeggio Controls
const int ARPBUTTON = 27; // Button
const int ARPLEDRED = 9; // Red LED
const int ARPLEDGRN = 8; // Green LED
const int ARPLEDBLU = 7; // Blue LED
const int ARPENCODERA = 35; // Encoder A pin
const int ARPENCODERB = 36; // Encoder B pin

// Glissando Controls
const int GLISSBUTTON = 28; // Button
const int GLISSLEDRED = 12; // Red LED
const int GLISSLEDGRN = 11; // Green LED
const int GLISSLEDBLU = 10; // Blue LED
const int GLISSENCODERA = 37; // Encoder A pin
const int GLISSENCODERB = 38; // Encoder B pin

// Volume Controls
const int VOLENCODERA = 33; // Encoder A pin
const int VOLENCODERB = 34; // Encoder B pin

/******************************************************************************/
/* Songbox Controls						                                      */
/******************************************************************************/
const int PLAYBUTTON = 29;
const int PLAYLED = 13;
const int PAUSBUTTON = 30;
const int PAUSLED = 44;
const int STOPBUTTON = 31;
const int STOPLED = 45;
const int RECDBUTTON = 32;
const int RECDLED = 46;
const int TEMPOENCODERA = 39;
const int TEMPOENCODERB = 40;

/******************************************************************************/
/* Numpad Pins							                                      */
/******************************************************************************/
const int ROW1 = A1;
const int ROW2 = A6;
const int ROW3 = A5;
const int ROW4 = A3;
const int COL1 = A2;
const int COL2 = A0;
const int COL3 = A4;

/******************************************************************************/
/* Screen Pins							                                      */
/******************************************************************************/
const int CS = 53;
const int DC = 49;
const int RST = 0;

// MIDI Handler
synth::ArduinoMIDIHandler * AMHandler;

// Pinout structs
chip::ModuleControlPins * modulePinout;
chip::KeypadPins * numPadPinout;
chip::ScreenPins * screenPinout;
chip::SongboxControlPins * songboxPinout;

// Controller classes
chip::ChipModuleControl * moduleController;
chip::ChipNumberPad * numPadController;
chip::ChipSongboxControl * songboxController;
chip::ChipScreenControl * screenController;

int currentModule;

void pinupModules() {

	modulePinout = new chip::ModuleControlPins;
	// Module Selectors
	modulePinout->redButtonPin = REDBUTTON;
	modulePinout->yelButtonPin = YELBUTTON;
	modulePinout->grnButtonPin = GRNBUTTON;
	modulePinout->bluButtonPin = BLUBUTTON;
	modulePinout->whtButtonPin = WHTBUTTON;
	modulePinout->redLEDPin = REDLED;
	modulePinout->yelLEDPin = YELLED;
	modulePinout->grnLEDPin = GRNLED;
	modulePinout->bluLEDPin = BLULED;
	modulePinout->whtLEDPin = WHTLED;

	// Arp pins
	modulePinout->arpButtonPin = ARPBUTTON;
	modulePinout->arpRedLEDPin = ARPLEDRED;
	modulePinout->arpGrnLEDPin = ARPLEDGRN;
	modulePinout->arpBluLEDPin = ARPLEDBLU;
	modulePinout->arpEncoderPinA = ARPENCODERA;
	modulePinout->arpEncoderPinB = ARPENCODERB;
	modulePinout->arpButtonPin = ARPBUTTON;
	// Gliss pins
	modulePinout->glissButtonPin = GLISSBUTTON;
	modulePinout->glissRedLEDPin = GLISSLEDRED;
	modulePinout->glissGrnLEDPin = GLISSLEDGRN;
	modulePinout->glissBluLEDPin = GLISSLEDBLU;
	modulePinout->glissEncoderPinA = GLISSENCODERA;
	modulePinout->glissEncoderPinB = GLISSENCODERB;
	// Volume pins
	modulePinout->volEncoderPinA = VOLENCODERA;
	modulePinout->volEncoderPinB = VOLENCODERB;
}

void pinupNumPad() {
	numPadPinout = new chip::KeypadPins;
	numPadPinout->row1Pin = ROW1;
	numPadPinout->row2Pin = ROW2;
	numPadPinout->row3Pin = ROW3;
	numPadPinout->row4Pin = ROW4;
	numPadPinout->col1Pin = COL1;
	numPadPinout->col2Pin = COL2;
	numPadPinout->col3Pin = COL3;

}

void pinupSongbox() {
	songboxPinout = new chip::SongboxControlPins;
	songboxPinout->pauseButtonPin = PAUSBUTTON;
	songboxPinout->playButtonPin = PLAYBUTTON;
	songboxPinout->stopButtonPin = STOPBUTTON;
	songboxPinout->recordButtonPin = RECDBUTTON;
	songboxPinout->pauseLEDPin = PAUSLED;
	songboxPinout->playLEDPin = PLAYLED;
	songboxPinout->stopLEDPin = STOPLED;
	songboxPinout->recordLEDPin = RECDLED;
	songboxPinout->tempoEncoderPinA = TEMPOENCODERA;
	songboxPinout->tempoEncoderPinB = TEMPOENCODERB;
}

void pinupScreen() {
	screenPinout = new chip::ScreenPins;
	screenPinout->cs = CS;
	screenPinout->dc = DC;
	screenPinout->rst = RST;

}

void setup() {
// Start serial comm for debugging
	Serial.begin(9600);
	Serial.println("Start");
	
	AMHandler = new synth::ArduinoMIDIHandler(&Serial1);
	AMHandler->begin();

	pinupScreen();
	screenController = new chip::ChipScreenControl(screenPinout);
	screenController->begin();

	pinupModules();
	moduleController = new chip::ChipModuleControl(modulePinout, AMHandler, screenController);
	moduleController->begin();

	pinupNumPad();
	numPadController = new chip::ChipNumberPad(numPadPinout, AMHandler, screenController);

	pinupSongbox();
	songboxController = new chip::ChipSongboxControl(songboxPinout, AMHandler, screenController);
	songboxController->begin();


	delete modulePinout;
	delete numPadPinout;
	delete songboxPinout;
}

void loop() {
	moduleController->poll();
	currentModule = moduleController->getCurrentModule();
	numPadController->poll(currentModule);
	songboxController->poll();
}

