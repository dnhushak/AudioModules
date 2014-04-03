#include "ChipNumberPad.h"

namespace chip {
	
//	-30;

//Keypad Initialization

	ChipNumberPad::ChipNumberPad(int * rowPins, int * colPins,
			synth::ArduinoMIDIHandler * initAMHandler) {
		// Initialize the keypad object
		byte rows = 4; //four rows
		byte cols = 3; //three columns
		char keys[rows][cols] = { { '1', '2', '3' }, { '4', '5', '6' }, { '7',
				'8', '9' }, { '*', '0', '#' } };
		keypad = new Keypad(makeKeymap(keys), rowPins, colPins, rows, cols);

		// Initialize key input info
		selectionValue = 0;
		key = '0';
		keyValue = 0;
		numKeysPressed = 0;
		digits[0] = digits[1] = digits[2] = 0;
		voiceOrSong = VOICE;

		// Initialize MIDI
		AMHandler = initAMHandler;
		message = new synth::MIDIMessage;
	}

	void ChipNumberPad::poll(int state) {
		//Get Keypress
		key = keypad->getKey();
		// ASCII to integer calculation (saves a LUT or stupid long switch case)
		keyValue = key - 30;

		/*
		 * Behavior: A slight bit convoluted but here we go.
		 *
		 * The number pad is going to select different songs/voices. It does so
		 * by sending progrom change values (for voices), or song select changes (for songs).
		 * The user can send program changes or song select messages from the same number pad
		 *
		 * To send a voice program change, the user types '*' <PROGRAM #> '*'
		 * To send a song selection change, the user types '#' <PROGRAM #> '#'
		 *
		 * Note that checks for valid values are done every key press:
		 * No values greater than 127 (so if two digits have been typed in and the first one is >1,
		 * it will not accept another one, or if the first one is 1 and the second one is greater than 3, etc.)
		 *
		 * After three numerical button presses this class will not accept any more keypresses.
		 *
		 * After an event has been sent, the keycount and digits reset. The keypad will not accept any keypresses
		 * other than a '*' or '#'. If the user wishes to cancel a command, they simply press the opposite operator.
		 * (So if they pressed '*', '1', '2', but decided the second digit needed to be something else, they press pound
		 * and the class escapes the sequence and resets the keycount and digits.
		 *
		 */
		if (key != NO_KEY) {
			//Check for # or *
			if (key == '#' || key == '*') {
				//If no keys pressed...
				if (!numKeysPressed) {
					//... change state to reflect voice or song (* for voice, # for song)
					switch (key) {
						case '*':
							voiceOrSong = VOICE;
							break;
						case '#':
							voiceOrSong = SONG;
					}
					// 1 key now pressed
					numKeysPressed++;
				}

				//If keys pressed, send voice or song message with keyval
				//Check for correct qualifier key pressed
				else if ((key == '*' && voiceOrSong == VOICE)
						|| (key == '#' && voiceOrSong == SONG)) {
					//Switch to correct MIDI channel - state for currently selected state, channel 15 for songbox
					byte channel;
					if (voiceOrSong) {
						//If Voice, set the channel to the input state
						message->channel = state;
						// Set the message to be a program change
						message->statusType = synth::PROGRAM;
					} else {
						// Song select system message (less significant nibble is 3)
						message->channel = synth::SONGSELECT;
						// Send a system message
						message->statusType = synth::SYSTEM;
					}

					// Set the program change number/song number to what has been typed
					message->data1 = selectionValue;

					// Send the MIDI
					AMHandler->writeMIDI(message);

					// Reset key info
					numKeysPressed = 0;
					digits[0] = digits[1] = digits[2] = 0;
				}
				//Press other one to cancel
				else if ((key == '#' && voiceOrSong == VOICE)
						|| (key == '*' && voiceOrSong == SONG)) {
					//reset digits
					numKeysPressed = 0;
					digits[0] = digits[1] = digits[2] = 0;
				}
			} else if (numKeysPressed > 0) {
				/**
				 *
				 *Check for correct inputs:
				 * Number of keys pressed < 3 (no more than 3 digits)
				 * Digits[1] < 2 (preventing anything 200 or greater from getting through)
				 * AND Digits[0] < 2 (preventing anything > 110)
				 * OR Digits[0] == 2 && new Key value < 8 (allowing up to 127)
				 */
				if ((numKeysPressed < 3)
						|| (digits[1] < 2
								&& (digits[0] < 2
										|| (digits[0] == 2 && keyValue < 8))
								&& numKeysPressed < 4)) {
					// Shift the digits
					digits[2] = digits[1];
					digits[1] = digits[0];
					digits[0] = keyValue;
					numKeysPressed++;
				}
			}

			// Set the keyed value to represent the digits typed
			selectionValue = (100 * digits[2]) + (10 * digits[1]) + (digits[0]);
		}
	}

	ChipNumberPad::~ChipNumberPad() {
		delete message;
		delete keypad;
	}
}
