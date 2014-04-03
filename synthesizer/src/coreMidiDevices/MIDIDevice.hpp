//MIDI Device Class
#ifndef MIDIDEVICE_H
#define MIDIDEVICE_H
#include <vector>
#include <iostream>
#include <stdint.h>
#include "Device.hpp"

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

	enum SystemType{
		SYSEX = 0,
		MTC = 1,
		SONGPOSITION = 2,
		SONGSELECT = 3,
		TUNEREQUEST = 6,
		SYSEXEND = 7,
		TIMECLOCK = 8,
		START = 10,
		CONTINUE = 11,
		STOP = 12,
		ACTIVESENSE = 14,
		RESET = 15
	};

	class MIDIDevice: public virtual Device {
		public:
			MIDIDevice();

			// The affect call used to act based on MIDI input
			virtual void affect(MIDIMessage *);

			// Adds MIDI object to mixer list
			void addMIDIDevice(MIDIDevice *);

			// Adds a vector of MIDI objects to mixer list
			void addMIDIDevices(std::vector<MIDIDevice *> *);

			// Changes the device list by pointing to a different one
			void setMIDIDeviceList(std::vector<MIDIDevice *> *);

			// Removes MIDI object from mixer list based on MIDI object reference
			void removeMIDIDevice(MIDIDevice *);

			// Removes MIDI object from mixer based on vector index
			void removeMIDIDevice(int);

			// Remove all objects from the mixer
			void removeAllMIDIDevices();

			// Returns the number of objects in this mixer
			int getNumMIDIDevices();

			// Sets the maximum number of devices
			int setMaxNumMIDIDevices();

			int getMIDIState();

			virtual ~MIDIDevice();
		protected:
			// TODO: Change the vector to list, to match audio device behavior
			// List of MIDI items to mix together
			std::vector<MIDIDevice *> * MIDIDeviceList;

			int MIDIstate;

			// Maximum number of MIDI devices allowable in effect (-1 -> no maximum)
			int maxNumMIDIDevices;

			// Actual number of MIDI devices in effect
			int numMIDIDevices;

			// Scale a MIDI message to an int
			int scaleValue(int value, int min, int max);

			// Scale a MIDI message to a float
			float scaleValue(int value, float min, float max);
	};
}
#endif /*MIDIDevice*/
