#include "MIDIUtils.h"

namespace midi {
	// Scale a MIDI message to an int
	int scaleValue(int value, int min, int max) {
		return (value * ((max - min) / 127) + min);
	}

	// Scale a MIDI message to a float
	float scaleValue(int value, float min, float max){
		return (value * ((max - min) / 127.0) + min);
	}
}
