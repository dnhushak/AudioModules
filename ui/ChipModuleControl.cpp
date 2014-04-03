#include "ChipModuleControl.h"

namespace chip {
	
	ChipModuleControl::ChipModuleControl(ModuleControlPins* pinout,
			synth::ArduinoMIDIHandler * initAMHandler) {
		AMHandler = initAMHandler;
		message = new synth::MIDIMessage;
		message->statusType = synth::CC;

		redButton = new ArduinoUI::Button(pinout->redButtonPin);
		yelButton = new ArduinoUI::Button(pinout->yelButtonPin);
		grnButton = new ArduinoUI::Button(pinout->grnButtonPin);
		bluButton = new ArduinoUI::Button(pinout->bluButtonPin);
		whtButton = new ArduinoUI::Button(pinout->whtButtonPin);
		redLED = new ArduinoUI::LED(pinout->redLEDPin);
		yelLED = new ArduinoUI::LED(pinout->yelLEDPin);
		grnLED = new ArduinoUI::LED(pinout->grnLEDPin);
		bluLED = new ArduinoUI::LED(pinout->bluLEDPin);
		whtLED = new ArduinoUI::LED(pinout->whtLEDPin);
		arpButton = new ArduinoUI::Button(pinout->arpButtonPin);
		arpLED = new ArduinoUI::RGBLED(pinout->arpRedLEDPin, pinout->arpGrnLEDPin, pinout->arpBluLEDPin);
		arpEncoder = new ArduinoUI::Encoder(pinout->arpEncoderPinA, pinout->arpEncoderPinB);
		glissButton = new ArduinoUI::Button(pinout->arpButtonPin);
		glissLED = new ArduinoUI::RGBLED(pinout->glissRedLEDPin, pinout->glissGrnLEDPin, pinout->glissBluLEDPin);
		glissEncoder = new ArduinoUI::Encoder(pinout->arpEncoderPinA, pinout->arpEncoderPinB);
		volumeEncoder = new ArduinoUI::Encoder(pinout->volEncoderPinA, pinout->volEncoderPinB);
		
		currentModule = RED;
		lastModule = RED;
		lastLEDModule = WHT;

	}
	
	ChipModuleControl::~ChipModuleControl() {
		delete redButton;
		delete yelButton;
		delete grnButton;
		delete bluButton;
		delete whtButton;
		delete redLED;
		delete yelLED;
		delete grnLED;
		delete bluLED;
		delete whtLED;
		delete arpButton;
		delete arpLED;
		delete arpEncoder;
		delete glissButton;
		delete glissLED;
		delete glissEncoder;
		delete volumeEncoder;
	}
	
	void ChipModuleControl::begin() {
		redButton->begin();
		yelButton->begin();
		grnButton->begin();
		bluButton->begin();
		whtButton->begin();
		redLED->begin();
		yelLED->begin();
		grnLED->begin();
		bluLED->begin();
		whtLED->begin();
		arpButton->begin();
		arpLED->begin();
		arpEncoder->begin();
		glissButton->begin();
		glissLED->begin();
		glissEncoder->begin();
		volumeEncoder->begin();

		arpEncoder->setCurrentVal(64);
		glissEncoder->setCurrentVal(64);
		volumeEncoder->setCurrentVal(64);
		updateLED();
	}
	
	void ChipModuleControl::poll() {
		/*
		 * Behavior:
		 *
		 * Pressing any of the five color buttons switches to that state
		 *
		 * Pressing arp or gliss turns that parameter off or on for the active state
		 *
		 * Turning any of the encoders affects the encoder's parameter for the active state
		 */

		if (currentModule == lastModule) {
			if (redButton->isPressed()) {
				currentModule = RED;
			}
		}
		updateLED();
	}
	
	void ChipModuleControl::updateLED() {
		if (currentModule != lastLEDModule) {
			switch (currentModule) {
				case RED:
					redLED->on();
					yelLED->off();
					grnLED->off();
					bluLED->off();
					whtLED->off();
					arpLED->setColor(255, 0, 0);
					glissLED->setColor(255, 0, 0);
					lastLEDModule = currentModule;
					break;
				case YEL:
					redLED->off();
					yelLED->on();
					grnLED->off();
					bluLED->off();
					whtLED->off();
					arpLED->setColor(255, 255, 0);
					glissLED->setColor(255, 255, 0);
					lastLEDModule = currentModule;
					break;
				case GRN:
					redLED->off();
					yelLED->off();
					grnLED->on();
					bluLED->off();
					whtLED->off();
					arpLED->setColor(0, 255, 0);
					glissLED->setColor(0, 255, 0);
					lastLEDModule = currentModule;
					break;
				case BLU:
					redLED->off();
					yelLED->off();
					grnLED->off();
					bluLED->on();
					whtLED->off();
					arpLED->setColor(0, 0, 255);
					glissLED->setColor(0, 0, 255);
					lastLEDModule = currentModule;
					break;
				case WHT:
					redLED->off();
					yelLED->off();
					grnLED->off();
					bluLED->off();
					whtLED->on();
					arpLED->setColor(255, 255, 255);
					glissLED->setColor(255, 255, 255);
					lastLEDModule = currentModule;
					break;
			}
		}

		if (arpState[currentModule]) {
			arpLED->blink(arpTime[currentModule]);
		}
		if (glissState[currentModule]) {
			arpLED->blink(glissTime[currentModule]);
		}
	}

}
