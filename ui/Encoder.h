#ifndef ENCODER_H_
#define ENCODER_H_
#include "Arduino.h"
namespace ArduinoUI {
	
	class Encoder {
		public:
			Encoder(int initPinA, int initPinB);
			void begin();
			void setMinVal(int newMinVal);
			void setMaxVal(int newMaxVal);
			void setCurrentVal(int newCurrentVal);
			int getCurrentVal();
			void poll();
			int hasChanged();

		private:
			// Check currentVal against bounds, making sure it's <max & >min
			void normalizeCurrentVal();
			// Pin assignments
			int pinA, pinB;
			// Encoder values
			int minVal, maxVal, currentVal, lastCheckedVal;
			// Encoder states
			int aLastState, bLastState, aPolledState, bPolledState;
	};

}

#endif /* ENCODER_H_ */
