#include "ChipSongboxControl.h"

namespace chip {
	
	ChipSongboxControl::ChipSongboxControl(SongboxControlPins* pinout,
			synth::ArduinoMIDIHandler* initAMHandler,
			chip::ChipScreenControl * initScreenController) {
		// MIDI Handling
		AMHandler = initAMHandler;
		MIDIStop.statusType = synth::SYSTEM;
		MIDIStart.statusType = synth::SYSTEM;
		MIDIContinue.statusType = synth::SYSTEM;
		MIDIRecord.statusType = synth::SYSTEM;
		MIDIStop.channel = synth::STOP;
		MIDIStart.channel = synth::START;
		MIDIContinue.channel = synth::CONTINUE;
		// Using undefined system message 9 for record
		MIDIRecord.channel = 9;
		MIDIStop.data1 = 0;
		MIDIStart.data1 = 0;
		MIDIContinue.data1 = 0;
		MIDIRecord.data1 = 0;
		MIDIStop.data2 = 0;
		MIDIStart.data2 = 0;
		MIDIContinue.data2 = 0;
		MIDIRecord.data2 = 0;

		// Button Initialization
		pauseButton = new ArduinoUI::Button(pinout->pauseButtonPin);
		playButton = new ArduinoUI::Button(pinout->playButtonPin);
		stopButton = new ArduinoUI::Button(pinout->stopButtonPin);
		recordButton = new ArduinoUI::Button(pinout->recordButtonPin);

		// LED Initialization
		pauseLED = new ArduinoUI::LED(pinout->pauseLEDPin);
		playLED = new ArduinoUI::LED(pinout->playLEDPin);
		stopLED = new ArduinoUI::LED(pinout->stopLEDPin);
		recordLED = new ArduinoUI::LED(pinout->recordLEDPin);

		// Encoder Initialization
		tempoEncoder = new ArduinoUI::Encoder(pinout->tempoEncoderPinA,
				pinout->tempoEncoderPinB);

		// Playback states
		playbackState = STOPPED;
		lastPlaybackStateLED = PLAYING;
		recordState = lastRecordState = OFF;
		tempo = 120;

		screenController = initScreenController;

	}
	
	void ChipSongboxControl::begin() {
		pauseButton->begin();
		playButton->begin();
		stopButton->begin();
		recordButton->begin();
		pauseLED->begin();
		playLED->begin();
		stopLED->begin();
		recordLED->begin();
		tempoEncoder->begin();

		// Set tempo encoder to values of beats per minute
		tempoEncoder->setMaxVal(300);
		tempoEncoder->setMinVal(20);

	}
	
	void ChipSongboxControl::poll() {
		/*
		 * Behavior:
		 *
		 * Pressing play while in stop mode starts from beginning Ã
		 * Pressing play while in pause mode continues Ã
		 * Pressing pause while in play mode pauses Ã
		 * Pressing pause while in pause mode does nothing Ã
		 * Pressing pause in stop mode does nothing Ã
		 * Pressing stop at any time stops Ã
		 * Pressing record while in play mode does nothing, Otherwise toggles record arm mode Ã
		 */
		pauseButton->pollDebounce();
		playButton->pollDebounce();
		stopButton->pollDebounce();
		recordButton->pollDebounce();

		switch (playbackState) {
			case PLAYING:
				if (stopButton->isPressed()) {
					playbackState = STOPPED;
					AMHandler->writeMIDI(&MIDIStop);
					screenController->writeTextTop("* STOP *");
					screenController->writeTextMid("");
					screenController->writeTextBot("");
				} else if (pauseButton->isPressed()) {
					playbackState = PAUSED;
					AMHandler->writeMIDI(&MIDIStop);
					screenController->writeTextTop("*PAUSE *");
					screenController->writeTextMid("");
					screenController->writeTextBot("");
				}
				break;
			case PAUSED:
				if (stopButton->isPressed()) {
					playbackState = STOPPED;
					screenController->writeTextTop("* STOP *");
					screenController->writeTextMid("");
					screenController->writeTextBot("");
				} else if (playButton->isPressed()) {
					playbackState = PLAYING;
					AMHandler->writeMIDI(&MIDIContinue);
					if (recordState == ARMED) {
						AMHandler->writeMIDI(&MIDIRecord);
						screenController->writeTextTop("*RECORD*");
						screenController->writeTextMid("");
						screenController->writeTextBot("");
					} else {
						screenController->writeTextTop("* PLAY *");
						screenController->writeTextMid("");
						screenController->writeTextBot("");
					}
				}
				setRecordState();
				break;
			case STOPPED:
				if (playButton->isPressed()) {
					playbackState = PLAYING;
					AMHandler->writeMIDI(&MIDIStart);
					if (recordState == ARMED) {
						AMHandler->writeMIDI(&MIDIRecord);
						screenController->writeTextTop("*RECORD*");
						screenController->writeTextMid("");
						screenController->writeTextBot("");
					} else {
						screenController->writeTextTop("* PLAY *");
						screenController->writeTextMid("");
						screenController->writeTextBot("");
					}
				}
				setRecordState();
				break;
		}
		updateLED();
	}

	void ChipSongboxControl::updateLED() {
		/*
		 * Behavior:
		 *
		 * In play mode, only play button is lit, and record is lit only if in record arm mode
		 * In stop mode, only stop button is lit, and record blinks only if in record arm mode
		 * In pause mode, pause button is lit, stop is not, play blinks, and record blinks only if in record arm mode
		 */
		switch (playbackState) {
			// In stopped state
			case STOPPED:
				// If in record arm mode, blink the record button
				if (recordState == ARMED) {
					recordLED->blink();
				}
				// If not in arm mode, turn record led off
				else {
					recordLED->off();
				}
				if (lastPlaybackStateLED != STOPPED) {
					// Turn the Play LED off
					playLED->off();
					// Turn the Stop LED on
					stopLED->on();
					// Turn the pause LED off
					pauseLED->off();
				}
				break;
			case PAUSED:
				// If in record arm mode, blink the record button
				if (recordState == ARMED) {
					recordLED->blink();
				}
				// If not in arm mode, turn record led off
				else {
					recordLED->off();
				}
				// Blink the playLED
				playLED->blink();
				if (lastPlaybackStateLED != PAUSED) {
					stopLED->off();
					pauseLED->on();
				}
				break;
			case PLAYING:
				// Playing mode
				if (recordState == ARMED) {
					// Record LED is on
					recordLED->on();
				} else {
					recordLED->off();
				}
				if (lastPlaybackStateLED != PLAYING) {
					playLED->on();
					stopLED->off();
					pauseLED->off();
				}
				break;
		}
		lastPlaybackStateLED = playbackState;

	}

	void ChipSongboxControl::setRecordState() {
		/*
		 * Four states here:
		 * -Armed and still pressed
		 * -Armed
		 * -Off and still pressed
		 * -Off
		 * Pressing in armed mode takes to off and still pressed
		 * Releasing in off and still pressed takes to off
		 * Pressing in off mode takes to armed and still pressed
		 * Releasing in armed and still pressed takes to armed
		 */
		if (recordState == ARMED) {
			// Button release in "Armed and still pressed"
			if (lastRecordState == OFF && !recordButton->isPressed()) {
				lastRecordState = ARMED;
			}
			// Button press in "Armed"
			else if (lastRecordState == ARMED && recordButton->isPressed()) {
				recordState = OFF;
				screenController->writeTextTop("*RECORD*");
				screenController->writeTextMid("DISARMED");
				screenController->writeTextBot("");
			}
		} else if (recordState == OFF) {
			// Button release in "Off and still pressed"
			if (lastRecordState == ARMED && !recordButton->isPressed()) {
				lastRecordState = OFF;
			}
			// Button press in "Off"
			else if (lastRecordState == OFF && recordButton->isPressed()) {
				recordState = ARMED;
				screenController->writeTextTop("*RECORD*");
				screenController->writeTextMid(" ARMED  ");
				screenController->writeTextBot("");
			}
		}
	}

	//TODO: Tempo Encoder and screen update of tempo
	/**
	 *
	 screenController->writeTextTop("Arp");
	 screenController->writeTextMid("Speed");
	 sprintf(buf, "%d", arpTime[currentModule]);
	 screenController->writeTextBot(buf);
	 */
	ChipSongboxControl::~ChipSongboxControl() {
		delete playButton;
		delete pauseButton;
		delete stopButton;
		delete recordButton;
		delete playLED;
		delete stopLED;
		delete pauseLED;
		delete recordLED;
		delete tempoEncoder;
	}
}
