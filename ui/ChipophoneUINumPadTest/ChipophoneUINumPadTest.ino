#include <Keypad.h>
#include <MIDI.h>
#define VOICE (0)
#define SONG (1)  

//From other code
byte state = 0;

//Keypad Initialization
const byte rows = 4; //four rows
const byte cols = 3; //three columns
char keys[rows][cols] = { { '1', '2', '3' }, { '4', '5', '6' },
		{ '7', '8', '9' }, { '*', '0', '#' } };
byte rowPins[rows] = { 41, 46, 45, 43 }; //connect to the row pinouts of the keypad
byte colPins[cols] = { 42, 40, 44 }; //connect to the column pinouts of the keypad
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, rows, cols);

int charToInt(char ch) {
	switch (ch) {
		case '0':
			return 0;
			break;
		case '1':
			return 1;
			break;
		case '2':
			return 2;
			break;
		case '3':
			return 3;
			break;
		case '4':
			return 4;
			break;
		case '5':
			return 5;
			break;
		case '6':
			return 6;
			break;
		case '7':
			return 7;
			break;
		case '8':
			return 8;
			break;
		case '9':
			return 9;
			break;
	}
	return -1;
}

void setup() {
	Serial.begin(9600);
}

void loop() {
	//Get Keypress
	char key = keypad.getKey();
	
	if (key != NO_KEY) {
		//Check for # or *
		if (key == '#' || key == '*') {
			//If no keys pressed, change state to reflect voice or song (* for voice, # for song)
			if (!keyspressed) {
				switch (key) {
					case '*':
						voiceOrSong = VOICE;
						break;
					case '#':
						voiceOrSong = SONG;
				}
				keyspressed++;
			}

			//If keys pressed, send voice or song message with keyval
			//Check for correct qualifier key pressed
			else if ((key == '*' && voiceOrSong == VOICE)
					|| (key == '#' && voiceOrSong == SONG)) {
				//Switch to correct MIDI channel - state for currently selected state, channel 15 for songbox
				byte channel;
				if (voiceOrSong) {
					//If Voice
					channel = state;
				} else {
					//If Song, for songbox
					channel = 15;
				}
				
				//Build MIDI Messages
				byte msg[2];
				msg[0] = (PROGRAMCHANGE << 4) | channel;
				msg[1] = keyedVal;
				
				//Send MIDI Messages
				Serial.print(msg[0], BIN);
				Serial.print("\n");
				Serial.print(msg[1]);
				Serial.print("\n");
				
				//reset digits
				keyspressed = 0;
				digits[0] = digits[1] = digits[2] = 0;
			}
			//Press other one to cancel
			else if ((key == '#' && voiceOrSong == VOICE)
					|| (key == '*' && voiceOrSong == SONG)) {
				Serial.print("\n***Cancel***\n");
				//reset digits
				keyspressed = 0;
				digits[0] = digits[1] = digits[2] = 0;
			}
		} else if (keyspressed > 0) {
			//Check for correct inputs
			if ((keyspressed < 3)
					|| (digits[1] < 2
							&& (digits[0] < 2
									|| (digits[0] == 2 && charToInt(key) < 8))
							&& keyspressed < 4)) {
				digits[2] = digits[1];
				digits[1] = digits[0];
				digits[0] = charToInt(key);
				keyspressed++;
			}
		}
		keyedVal = (100 * digits[2]) + (10 * digits[1]) + (digits[0]);
	}
}

