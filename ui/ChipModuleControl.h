#ifndef CHIPMODULECONTROL_H_
#define CHIPMODULECONTROL_H_
#include "Button.h"
#include "Encoder.h"
#include "LED.h"
#include "RGBLED.h"
#include "ArduinoMIDIHandler.h"

namespace chip {
	typedef struct ModuleControlPins {
			// Module Selector Buttons
			int redButtonPin;
			int yelButtonPin;
			int grnButtonPin;
			int bluButtonPin;
			int whtButtonPin;
			// Module Selector LEDs
			int redLEDPin;
			int yelLEDPin;
			int grnLEDPin;
			int bluLEDPin;
			int whtLEDPin;
			// Arpeggiator button, encoder and LEDs
			int arpButtonPin;
			int arpRedLEDPin;
			int arpGrnLEDPin;
			int arpBluLEDPin;
			int arpEncoderPinA;
			int arpEncoderPinB;
			// Glissando button, encoder and LEDs
			int glissButtonPin;
			int glissRedLEDPin;
			int glissGrnLEDPin;
			int glissBluLEDPin;
			int glissEncoderPinA;
			int glissEncoderPinB;
			// Volume encoder
			int volEncoderPinA;
			int volEncoderPinB;
	} ModuleControlPins;
	
	enum moduleState {
		RED, YEL, GRN, BLU, WHT
	};

	class ChipModuleControl: public synth::MIDIDevice {
		public:
			ChipModuleControl(ModuleControlPins* pinout,
					synth::ArduinoMIDIHandler * initAMHandler);
			virtual ~ChipModuleControl();
			void begin();
			void poll();
			int getCurrentModule();
		private:
			enum paramState {
				OFF, ON
			};
			enum ChipCCNums {
				ARPTOGGLE = 66,
				GLISSTOGGLE = 65,
				GLISSTIME = 5,
				VOLUME = 7,
				ARPTIME = 12
			};
			void updateLED();
			void calcArpTime();
			void calcGlissTime();
			// Module Selection
			ArduinoUI::Button * redButton;
			ArduinoUI::Button * yelButton;
			ArduinoUI::Button * grnButton;
			ArduinoUI::Button * bluButton;
			ArduinoUI::Button * whtButton;
			ArduinoUI::LED * redLED;
			ArduinoUI::LED * yelLED;
			ArduinoUI::LED * grnLED;
			ArduinoUI::LED * bluLED;
			ArduinoUI::LED * whtLED;
			// Arpeggio
			ArduinoUI::Button * arpButton;
			ArduinoUI::RGBLED * arpLED;
			ArduinoUI::Encoder * arpEncoder;
			// Glissando
			ArduinoUI::Button * glissButton;
			ArduinoUI::RGBLED * glissLED;
			ArduinoUI::Encoder * glissEncoder;
			// Volume
			ArduinoUI::Encoder * volumeEncoder;

			//MIDI Handling
			synth::ArduinoMIDIHandler * AMHandler;
			synth::MIDIMessage * message;

			// State tracking (lastLEDModule is used to keep track of LED updates)
			int currentModule, lastLEDModule;
			int arpState[5];
			int glissState[5];
			int arpTime[5];
			int glissTime[5];
			int volume[5];

	}
	;

}

#endif /* CHIPMODULECONTROL_H_ */
