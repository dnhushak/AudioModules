#ifndef CHIPSONGBOXCONTROL_H_
#define CHIPSONGBOXCONTROL_H_
#include "Arduino.h"
#include "Button.h"
#include "Encoder.h"
#include "LED.h"
#include "ArduinoMIDIHandler.h"
#include "ChipScreenControl.h"

namespace chip {
	typedef struct SongboxControlPins {
			int pauseButtonPin;
			int playButtonPin;
			int stopButtonPin;
			int recordButtonPin;
			int pauseLEDPin;
			int playLEDPin;
			int stopLEDPin;
			int recordLEDPin;
			int tempoEncoderPinA;
			int tempoEncoderPinB;
	} SongboxControlPins;
	
	class ChipSongboxControl {
		public:
			ChipSongboxControl(SongboxControlPins * pinout,
					synth::ArduinoMIDIHandler * initAMHandler,
					chip::ChipScreenControl * initScreenController);
			void begin();
			void poll();
			virtual ~ChipSongboxControl();
		private:
			enum SongboxState {
				STOPPED, PLAYING, PAUSED
			};

			enum SongboxRecordState {
				OFF, ARMED
			};
			void updateLED();
			void setRecordState();
			ArduinoUI::Button * pauseButton;
			ArduinoUI::Button * playButton;
			ArduinoUI::Button * stopButton;
			ArduinoUI::Button * recordButton;
			ArduinoUI::LED * pauseLED;
			ArduinoUI::LED * playLED;
			ArduinoUI::LED * stopLED;
			ArduinoUI::LED * recordLED;
			ArduinoUI::Encoder * tempoEncoder;
			synth::ArduinoMIDIHandler * AMHandler;
			synth::MIDIMessage MIDIStop, MIDIStart, MIDIContinue, MIDIRecord;
			int playbackState, lastPlaybackStateLED;
			int recordState, lastRecordState;
			int tempo;
			// Screen Handler
			ChipScreenControl * screenController;
			char buf[10];
	};

}

#endif /* CHIPSONGBOXCONTROL_H_ */
