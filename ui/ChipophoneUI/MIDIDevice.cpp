#include "MIDIDevice.h"

namespace synth {
	MIDIDevice::MIDIDevice() {
	}

// Forwards all MIDI messages in the stream to all in the MIDIDevice List
	void MIDIDevice::affect(MIDIMessage * message) {
	}

// Scale a MIDI message to an int
	int MIDIDevice::scaleValue(int value, int min, int max) {
		return (value * ((max - min) / 127) + min);
	}

// Scale a MIDI message to a float
	float MIDIDevice::scaleValue(int value, float min, float max) {
		return (value * ((max - min) / 127) + min);
	}

	MIDIDevice::~MIDIDevice() {
	}
}

