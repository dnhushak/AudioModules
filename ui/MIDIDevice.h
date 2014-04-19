//MIDI Device Class
#ifndef MIDIDEVICE_H
#define MIDIDEVICE_H
#include "MIDIUtils.h"
#include "Device.h"

namespace synth {

	class MIDIDevice: public virtual Device {
		public:
			MIDIDevice();

			// The affect call used to act based on MIDI input
			virtual void affect(MIDIMessage *) = 0;

			virtual ~MIDIDevice();
	};
}
#endif /*MIDIDevice*/
