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
		arpLED = new ArduinoUI::RGBLED(pinout->arpRedLEDPin,
				pinout->arpGrnLEDPin, pinout->arpBluLEDPin);
		arpEncoder = new ArduinoUI::Encoder(pinout->arpEncoderPinA,
				pinout->arpEncoderPinB);

		glissButton = new ArduinoUI::Button(pinout->glissButtonPin);
		glissLED = new ArduinoUI::RGBLED(pinout->glissRedLEDPin,
				pinout->glissGrnLEDPin, pinout->glissBluLEDPin);
		glissEncoder = new ArduinoUI::Encoder(pinout->glissEncoderPinA,
				pinout->glissEncoderPinB);

		volumeEncoder = new ArduinoUI::Encoder(pinout->volEncoderPinA,
				pinout->volEncoderPinB);
		
		for (int i = 0; i < 5; i++) {
			arpState[i] = OFF;
			arpTime[i] = 550;
			glissState[i] = OFF;
			glissTime[i] = 550;
			volume[i] = 64;
		}
		currentModule = RED;
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
		redButton->pollDebounce();
		yelButton->pollDebounce();
		grnButton->pollDebounce();
		bluButton->pollDebounce();
		whtButton->pollDebounce();
		arpButton->pollDebounce();
		arpEncoder->poll();
		glissButton->pollDebounce();
		glissEncoder->poll();
		volumeEncoder->poll();

		// Check buttons and change module accordingly
		if (redButton->hasChanged() && redButton->isPressed()) {
			currentModule = RED;
		}
		if (yelButton->hasChanged() && yelButton->isPressed()) {
			currentModule = YEL;
		}
		if (grnButton->hasChanged() && grnButton->isPressed()) {
			currentModule = GRN;
		}
		if (bluButton->hasChanged() && bluButton->isPressed()) {
			currentModule = BLU;
		}
		if (whtButton->hasChanged() && whtButton->isPressed()) {
			currentModule = WHT;
		}

		// Check arp button
		if (arpButton->hasChanged() && arpButton->isPressed()) {
			arpState[currentModule] = arpButton->isPressed()
					^ arpState[currentModule];
			message->channel = currentModule;
			message->data1 = ARPTOGGLE;
			message->data2 = arpState[currentModule] * 127;
			AMHandler->writeMIDI(message);
		}

		// Check gliss button
		if (glissButton->hasChanged() && glissButton->isPressed()) {
			glissState[currentModule] = glissButton->isPressed()
					^ glissState[currentModule];
			message->channel = currentModule;
			message->data1 = GLISSTOGGLE;
			message->data2 = glissState[currentModule] * 127;
			AMHandler->writeMIDI(message);
		}

		 if (arpEncoder->hasChanged()) {
		 arpTime[currentModule] = scaleValue(arpEncoder->getCurrentVal(), 50, 1000);
		 message->channel = currentModule;
		 message->data1 = ARPTIME;
		 message->data2 = arpEncoder->getCurrentVal();
		 AMHandler->writeMIDI(message);
		 }
		/*

		 if (glissEncoder->hasChanged()) {
		 glissTime[currentModule] = scaleValue(glissEncoder->getCurrentVal(), 50, 1000);
		 message->channel = currentModule;
		 message->data1 = GLISSTIME;
		 message->data2 = glissEncoder->getCurrentVal();
		 AMHandler->writeMIDI(message);
		 }

		 if (volumeEncoder->hasChanged()) {
		 volume[currentModule] = volumeEncoder->getCurrentVal();
		 message->channel = currentModule;
		 message->data1 = VOLUME;
		 message->data2 = volume[currentModule];
		 AMHandler->writeMIDI(message);
		 }
		 */
		updateLED();
	}
	
	int ChipModuleControl::getCurrentModule() {
		return currentModule;
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

		if (arpState[currentModule] == ON) {
			arpLED->blink(arpTime[currentModule]);
		} else {
			arpLED->off();
		}
		if (glissState[currentModule] == ON) {
			glissLED->blink(glissTime[currentModule]);
		} else {
			glissLED->off();
		}
	}

}
