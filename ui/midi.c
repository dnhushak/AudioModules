#include "midi.h"
//  plays a MIDI note.  Doesn't check to see that
//  cmd is greater than 127, or that data values are  less than 127:
void sendMidi(byte message, byte data1, byte data2) {
	Serial1.write(message);
	Serial1.write(data1);
	Serial1.write(data2);
}
