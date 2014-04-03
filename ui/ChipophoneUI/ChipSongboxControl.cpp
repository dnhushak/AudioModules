#include "ChipSongboxControl.h"

namespace chip {
	
	ChipSongboxControl::ChipSongboxControl(SongboxControlPins* initPins,
			synth::ArduinoMIDIHandler* initAMHandler) {
		// MIDI Handling
		AMHandler = initAMHandler;
		message = new synth::MIDIMessage;
		message->statusType = synth::SYSTEM;

		// Button Initialization
		pauseButton = new ArduinoUI::Button(initPins->pauseButtonPin);
		playButton = new ArduinoUI::Button(initPins->playButtonPin);
		stopButton = new ArduinoUI::Button(initPins->stopButtonPin);
		recordButton = new ArduinoUI::Button(initPins->recordButtonPin);

		// LED Initialization
		pauseLED = new ArduinoUI::LED(initPins->pauseLEDPin);
		playLED = new ArduinoUI::LED(initPins->playLEDPin);
		stopLED = new ArduinoUI::LED(initPins->stopLEDPin);
		recordLED = new ArduinoUI::LED(initPins->recordLEDPin);

		// Encoder Initialization
		tempoEncoder = new ArduinoUI::Encoder(initPins->tempoEncoderPinA,
				initPins->tempoEncoderPinB);

		// Playback states
		playbackState = lastPlaybackState = STOPPED;
		recordState = lastRecordState = OFF;
		tempo = 120;

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
		 * Pressing record while in play mode does nothing, Otherwise toggles record arm mode
		 */
		pauseButton->poll();
		playButton->poll();
		stopButton->poll();
		recordButton->poll();

		switch (playbackState) {
			case PLAYING:
				if (stopButton->isPressed()) {
					playbackState = STOPPED;
				}
				else if(pauseButton->isPressed()){
					playbackState = PAUSED;
				}
				break;
			case PAUSED:
				if (stopButton->isPressed()) {
					playbackState = STOPPED;
				}
				if (playButton->isPressed()) {
					playbackState = PLAYING;
				}
				break;
			case STOPPED:
				if (playButton->isPressed()) {
					playbackState = PLAYING;
				}
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
				// Turn the Play LED off
				playLED->off();
				// Turn the Stop LED on
				stopLED->on();
				// Turn the pause LED off
				pauseLED->off();
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
				break;
			case PLAYING:
				// Playing mode
				if (recordState == ARMED) {
					// Record LED is on
					recordLED->on();
				} else {
					recordLED->off();
				}
				playLED->on();
				stopLED->off();
				pauseLED->off();
				break;
		}

	}
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
