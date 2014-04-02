#include "Arduino.h"
#include <HardwareSerial.h>
#include "Button.h"
#include "ChannelSelector.h"
#include "LED.h"
#include "RGBLED.h"
#include "ArduinoMIDIHandler.h"
#include "Encoder.h"

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
const int RECDBUTTON  =A10;
const int RECDLED = 28;
const int TEMPOENCODERA=  8;
const int TEMPOENCODERB=  9;

int numChannels = 5;
ArduinoUI::Button * buttonA;
ArduinoUI::ChannelSelector * channelSel;
ArduinoUI::LED ** channelLED;
ArduinoUI::RGBLED * arpLED;
ArduinoUI::RGBLED * glissLED;
ArduinoUI::Encoder * VolEncoder;

synth::ArduinoMIDIHandler * AMHandler;

int channelSelPins[5] =
		{ REDBUTTON, YELBUTTON, GRNBUTTON, BLUBUTTON, WHTBUTTON };
int channelSelLEDs[5] = { REDLED, YELLED, GRNLED, BLULED, WHTLED };
int currentState;
int lastState;

void setup() {
	// Start serial comm for debugging
	Serial.begin(9600);
	Serial.println("Start");
	
	// Initialize the LED's
	channelLED = (ArduinoUI::LED **) malloc(
			sizeof(ArduinoUI::LED *) * numChannels);
	
	for (int i = 0; i < numChannels; i++) {
		channelLED[i] = new ArduinoUI::LED(channelSelLEDs[i]);
		channelLED[i]->begin();
		channelLED[i]->on();
	}
	
	arpLED = new ArduinoUI::RGBLED(ARPLEDRED, ARPLEDGRN, ARPLEDBLU);
	glissLED = new ArduinoUI::RGBLED(GLISSLEDRED, GLISSLEDGRN, GLISSLEDBLU);
	arpLED->begin();
	glissLED->begin();
	arpLED->on();
	glissLED->on();
	
	// Initialize the buttons
	channelSel = new ArduinoUI::ChannelSelector(channelSelPins, numChannels);
	channelSel->begin();
	buttonA = new ArduinoUI::Button(A10);
	buttonA->begin();
	
	// Initialize MIDI
	AMHandler = new synth::ArduinoMIDIHandler(&Serial1);
	AMHandler->begin();

	VolEncoder = new ArduinoUI::Encoder(TEMPOENCODERA, TEMPOENCODERB);
	VolEncoder->begin();
}

void loop() {
//	channelSel->poll();
//	currentState = channelSel->getState();
//	if (currentState != lastState) {
//		lastState = currentState;
//		Serial.println(currentState, DEC);
//		synth::MIDIMessage * msg = new synth::MIDIMessage;
//		msg->channel = currentState;
//		msg->statusType = synth::CC;
//		msg->data1 = 100;
//		msg->data2 = 25;
//		AMHandler->writeMIDI(msg);
//		for (int i = 0; i < numChannels; i++) {
//			channelLED[i]->off();
//			if (i == currentState) {
//				channelLED[i]->on();
//			}
//		}
//	}
	VolEncoder->poll();
	

}

