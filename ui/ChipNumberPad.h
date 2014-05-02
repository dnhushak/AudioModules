#ifndef NUMBERPAD_H_
#define NUMBERPAD_H_
#include <Keypad.h>
#include "Arduino.h"
#include "ArduinoMIDIHandler.h"
#include "Keypad.h"
#include "ChipScreenControl.h"

namespace chip {
	/*
	 * This is the number pad controller for the Chipophone
	 * This is NOT a scalable, reusable class,
	 * as there are a number of things specific to the ISU Chipohphone setup
	 */
	typedef struct KeypadPins {
			int row1Pin;
			int row2Pin;
			int row3Pin;
			int row4Pin;
			int col1Pin;
			int col2Pin;
			int col3Pin;
	} KeypadPins;

	class ChipNumberPad {
			enum {
				VOICE, SONG
			};
		public:
			ChipNumberPad(KeypadPins * pinout,
					synth::ArduinoMIDIHandler * initAMHandler,
					chip::ChipScreenControl * initScreenController);
			void poll(int state);
			~ChipNumberPad();

		private:
			void UpdateScreen(int voiceOrSong, int num);
			void CancelScreen();
			void SentScreen(int voiceOrSong, int num);

			// Selection of voice or song state
			// Ultimately just changes what MIDI Channels it sends
			int voiceOrSong;

			// The running collection of digits pressed
			int digits[3];

			// Number of keys pressed within a valid time
			int numKeysPressed;

			// The current value of successive keypresses
			// Pressing 2, 3, 6 will fill digits with 2, 3, and 6,
			// and set this value to 236
			int selectionValue;

			// The keypad object to poll
			Keypad * keypad;
			// The polled key
			char key;
			// The value of the polled key
			int keyValue;

			byte rows;
			byte cols;
			char * keys;
			byte * rowPins;
			byte * colPins;

			// The Arduino MIDI Handler to handle MIDI out
			synth::ArduinoMIDIHandler * AMHandler;

			// The message to send
			synth::MIDIMessage * message;

			// Screen Handler
			ChipScreenControl * screenController;
			char buf[10];
	};

}

#endif /* NUMBERPAD_H_ */
