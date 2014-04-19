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
const int ARPLEDRED = 7; // Red LED
const int ARPLEDGRN = 8; // Green LED
const int ARPLEDBLU = 9; // Blue LED
const int ARPENCODERA = 35; // Encoder A pin
const int ARPENCODERB = 36; // Encoder B pin

// Glissando Controls
const int GLISSBUTTON = 28; // Button
const int GLISSLEDRED = 10; // Red LED
const int GLISSLEDGRN = 11; // Green LED
const int GLISSLEDBLU = 12; // Blue LED
const int GLISSENCODERA = 37; // Encoder A pin
const int GLISSENCODERB = 38; // Encoder B pin

// Volume Controls
const int VOLENCODERA = 33; // Encoder A pin
const int VOLENCODERB = 34; // Encoder B pin

/******************************************************************************/
/* Songbox Controls						                                      */
/******************************************************************************/
const int PLAYBUTTON = A9;
const int PLAYLED = 26;
const int PAUSBUTTON = A8;
const int PAUSLED = 25;
const int STOPBUTTON = A6;
const int STOPLED = 27;
const int RECDBUTTON = A10;
const int RECDLED = 28;
const int TEMPOENCODERA = 8;
const int TEMPOENCODERB = 9;

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

// MIDI Handler
synth::ArduinoMIDIHandler * AMHandler;

// Pinout structs
chip::ModuleControlPins * modulePinout;
chip::KeypadPins * numPadPinout;
chip::SongboxControlPins * songboxPinout;

// Controller classes
chip::ChipModuleControl * moduleController;
chip::ChipNumberPad * numPadController;
chip::ChipSongboxControl * songboxController;

int currentModule;

ArduinoUI::Button * recordButton;
int presses=0;

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

void setup() {
// Start serial comm for debugging
	Serial.begin(9600);
	Serial.println("Start");
	
	AMHandler = new synth::ArduinoMIDIHandler(&Serial1);
	AMHandler->begin();

	pinupModules();
	moduleController = new chip::ChipModuleControl(modulePinout, AMHandler);
	moduleController->begin();

	pinupNumPad();
	numPadController = new chip::ChipNumberPad(numPadPinout, AMHandler);

	pinupSongbox();
	songboxController = new chip::ChipSongboxControl(songboxPinout, AMHandler);
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

