#include "Arduino.h"
#include "Button.h"

/*
 * Channel selector uses an array of buttons to
 * determine a single channel or state based on the last button pressed
 */
namespace UI {
	class ChannelSelector {
		public:
			ChannelSelector(int * initPins, int initNumModules);
			void begin();
			void poll();
			void pollDebounce();
			int getState();
			~ChannelSelector();

		private:
			// Num
			int numChannels;
			// Array of pointers to buttons
			Button * * buttons;
			int state;
	};

}
