//MIDI Device Class
#ifndef MIDIDEVICE_H
#define MIDIDEVICE_H
#include "Arduino.h"
#include "Device.h"

namespace synth {
	struct MIDIMessage {
			unsigned char statusType;
			unsigned char channel;
			unsigned char data1;
			unsigned char data2;
			int32_t time;
	};

	enum StatusType {
		NOTEOFF = 8,
		NOTEON = 9,
		POLYTOUCH = 10,
		CC = 11,
		PROGRAM = 12,
		MONOTOUCH = 13,
		PITCHBEND = 14,
		SYSTEM = 15
	};

	class MIDIDevice: public virtual Device {
		public:
			MIDIDevice();

			// The affect call used to act based on MIDI input
			virtual void affect(MIDIMessage *);

			virtual ~MIDIDevice();
		protected:
			// Scale a MIDI message to an int
			int scaleValue(int value, int min, int max);

			// Scale a MIDI message to a float
			float scaleValue(int value, float min, float max);
	};
}
#endif /*MIDIDevice*/

