#ifndef CHIPSONGBOXCONTROL_H_
#define CHIPSONGBOXCONTROL_H_
#include "Arduino.h"
#include "Button.h"
#include "Encoder.h"
#include "LED.h"

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
	
	enum SongboxState{
		STOPPED, PLAYING, PAUSED
	};

	enum SongboxRecordState{
		OFF, ARMED
	};

	class ChipSongboxControl {
		public:
			ChipSongboxControl(SongboxControlPins * initPins, synth::ArduinoMIDIHandler * initAMHandler);
			void begin();
			void poll();
			virtual ~ChipSongboxControl();
		private:
			void updateLED();
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
			synth::MIDIMessage * message;
			int playbackState, lastPlaybackState;
			int recordState, lastRecordState;
			int tempo;
	};

}

#endif /* CHIPSONGBOXCONTROL_H_ */
