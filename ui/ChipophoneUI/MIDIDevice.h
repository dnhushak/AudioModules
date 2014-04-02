#include <StandardCplusplus.h>
#include <system_configuration.h>
#include <unwind-cxx.h>
#include <utility.h>

//MIDI Device Class
#ifndef MIDIDEVICE_H
#define MIDIDEVICE_H
#include "Arduino.h"
#include <vector>
#include <iostream>
#include <stdint.h>
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

    int getMIDIState();

    virtual ~MIDIDevice();
  protected:

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

