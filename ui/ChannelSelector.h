#include "Arduino.h"
#include "Button.h"

/*
 * Channel selector uses an array of buttons to
 * determine a single channel or state based on the last button pressed
 */
namespace ArduinoUI {
	class ChannelSelector {
		public:
			ChannelSelector(int * initPins, int initNumChannels);
			void begin();
			void poll();
			void pollDebounce();
			int getState();
			~ChannelSelector();

		private:
			int numChannels;
			// Array of pointers to buttons
			Button * * buttons;
			int state;
	};

}
