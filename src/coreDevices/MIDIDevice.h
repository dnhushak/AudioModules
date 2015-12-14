//MIDI Device Class
#ifndef MIDIDEVICE_H
#define MIDIDEVICE_H
#include "MIDIUtils.h"
#include "Device.h"

namespace midi {

	class MIDIDevice: public virtual device::Device {
		public:
			MIDIDevice();

			/**
			 * Creates a deep copy of the current device, excluding buffer contents and device ID
			 * @return A copy of the device
			 */
			virtual MIDIDevice * clone(int) = 0;

			// The affect call used to act based on MIDI input
			virtual void affect(MIDIMessage *) = 0;

			virtual ~MIDIDevice();
	};
}
#endif /*MIDIDevice*/
