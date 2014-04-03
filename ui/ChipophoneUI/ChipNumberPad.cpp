#include "ChipNumberPad.h"

namespace chip {
	
//	-30;

//Keypad Initialization

	ChipNumberPad::ChipNumberPad(int * rowPins, int * colPins, synth::ArduinoMIDIHandler * initAMHandler) {
		// Initialize the keypad object
		byte rows = 4; //four rows
		byte cols = 3; //three columns
		char keys[rows][cols] = { { '1', '2', '3' }, { '4', '5', '6' }, { '7',
				'8', '9' }, { '*', '0', '#' } };
		keypad = new Keypad(makeKeymap(keys), rowPins, colPins, rows, cols);

		// Initialize key input info
		keyedVal = 0;
		numKeysPressed = 0;
		digits[0] = digits[1] = digits[2] = 0;
		voiceOrSong = VOICE;

		// Initialize MIDI
		AMHandler = initAMHandler;
		message = new synth::MIDIMessage;
		// Set the message to be a program change
		message->statusType = synth::PROGRAM;
	}

	void ChipNumberPad::poll(int state) {
		//Get Keypress
		char key = keypad->getKey();

		/*
		 * Behavior: A slight bit convoluted but here we go.
		 *
		 * The number pad is going to select different songs/voices. It does so
		 * by sending progrom change values.
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
						//If Voice
						message->channel = state;
					} else {
						//If Song, for songbox
						message->channel = 15;
					}

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
				//Check for correct inputs
				if ((numKeysPressed < 3)
						|| (digits[1] < 2
								&& (digits[0] < 2
										|| (digits[0] == 2 && charToInt(key) < 8))
								&& numKeysPressed < 4)) {
					digits[2] = digits[1];
					digits[1] = digits[0];
					digits[0] = charToInt(key);
					numKeysPressed++;
				}
			}

			// Set the keyed value to represent the digits typed
			keyedVal = (100 * digits[2]) + (10 * digits[1]) + (digits[0]);
		}
	}

	ChipNumberPad::~ChipNumberPad(){
		delete message;
		delete keypad;
	}
}
