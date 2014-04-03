#ifndef NUMBERPAD_H_
#define NUMBERPAD_H_
#include <Keypad.h>

namespace chip {
	/*
	 * This is the number pad controller for the Chipophone
	 * This is NOT a scalable, reusable class,
	 * as there are a number of things specific to the ISU Chipohphone setup
	 */

	class ChipNumberPad {
			enum {
				VOICE, SONG
			};
		public:
			ChipNumberPad(int * rowPins, int * colPins,
					synth::ArduinoMIDIHandler * initAMHandler);
			void poll(int state);
			~ChipNumberPad();

		private:
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
			int keyedVal;

			// The keypad object to poll
			Keypad * keypad;

			// The Arduino MIDI Handler to handle MIDI out
			synth::ArduinoMIDIHandler * AMHandler;

			// The message to send
			synth::MIDIMessage * message;
	};

}

#endif /* NUMBERPAD_H_ */
