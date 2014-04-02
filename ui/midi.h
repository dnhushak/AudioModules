#ifndef MIDI_H_
#define MIDI_H_

//Glissando control number and toggle number (both CC numbers)
#define GLISSCCNUM  (0b00000101)
#define GLISSTOGGLE (0b01000001)

//Arpeggio control number and toggle number (both CC numbers)
#define ARPEGCCNUM  (0b01010001)
#define ARPEGTOGGLE (0b01010000)

enum Status {
	NOTEOFF = 8,
	NOTEON = 9,
	POLYTOUCH = 10,
	CC = 11,
	PROGRAM = 12,
	MONOTOUCH = 13,
	PITCHBEND = 14,
	SYSTEM = 15
};


void sendMidi(byte message, byte data1, byte data2);

#endif /* MIDI_H_ */
