#include "Arduino.h"
#include "HardwareSerial.h"
#include <Keypad.h>
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
const int REDBUTTON = A1;
const int YELBUTTON = A0;
const int GRNBUTTON = A3;
const int BLUBUTTON = A2;
const int WHTBUTTON = A5;

const int REDLED = 39;
const int YELLED = 38;
const int GRNLED = 37;
const int BLULED = 36;
const int WHTLED = 35;

/******************************************************************************/
/* Channel Edit Buttons, Encoders and LEDs                                    */
/******************************************************************************/

// Arpgeggio Controls
const int ARPBUTTON = A4; // Button
const int ARPLEDRED = 34; // Red LED
const int ARPLEDGRN = 33; // Green LED
const int ARPLEDBLU = 32; // Blue LED
const int ARPENCODERA = 4; // Encoder A pin
const int ARPENCODERB = 5; // Encoder B pin

// Glissando Controls
const int GLISSBUTTON = A7; // Button
const int GLISSLEDRED = 31; // Red LED
const int GLISSLEDGRN = 30; // Green LED
const int GLISSLEDBLU = 29; // Blue LED
const int GLISSENCODERA = 6; // Encoder A pin
const int GLISSENCODERB = 7; // Encoder B pin

// Volume Controls
const int VOLENCODERA = 2; // Encoder A pin
const int VOLENCODERB = 3; // Encoder B pin

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
const int ROW1 = 41;
const int ROW2 = 46;
const int ROW3 = 45;
const int ROW4 = 43;
const int COL1 = 42;
const int COL2 = 40;
const int COL3 = 44;

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
	currentModule = moduleController->getState();
	numPadController->poll(currentModule);
	songboxController->poll();
}

