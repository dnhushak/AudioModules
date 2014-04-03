#include "ChipSongboxControl.h"

namespace chip {
	
	ChipSongboxControl::ChipSongboxControl(SongboxControlPins* initPins,
			synth::ArduinoMIDIHandler* initAMHandler) {
		AMHandler = initAMHandler;
		pauseButton = new ArduinoUI::Button(initPins->pauseButtonPin);
		playButton = new ArduinoUI::Button(initPins->playButtonPin);
		stopButton = new ArduinoUI::Button(initPins->stopButtonPin);
		recordButton = new ArduinoUI::Button(initPins->recordButtonPin);
		pauseLED = new ArduinoUI::LED(initPins->pauseLEDPin);
		playLED = new ArduinoUI::LED(initPins->playLEDPin);
		stopLED = new ArduinoUI::LED(initPins->stopLEDPin);
		recordLED = new ArduinoUI::LED(initPins->recordLEDPin);
		tempoEncoder = new ArduinoUI::Encoder(initPins->tempoEncoderPinA,
				initPins->tempoEncoderPinB);

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
		pauseButton->poll();
		playButton->poll();
		stopButton->poll();
		recordButton->poll();

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
				if( recordState == ARMED){
					// Record LED is on
					recordLED->on();
				}
				else{
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
