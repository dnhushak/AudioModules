/*
 * MIDIProcessor.h
 *
 *  Created on: Feb 2, 2017
 *      Author: dnhushak
 */

#ifndef SRC_COREMIDIDEVICES_MIDIPROCESSOR_H_
#define SRC_COREMIDIDEVICES_MIDIPROCESSOR_H_

#include "MIDIDevice.h"
#include "Connectable.h"

namespace midi{
using namespace device;
using namespace std;

class MIDIProcessor: public Connectable<MIDIDevice,Device>{

	public:
		MIDIProcessor();

//		virtual MIDIProcessor * clone(int);
//
//		void process(MIDIMessage *);
//
//		void setFunction(int statusType, int paramNumber, int scaleLow, int scaleHi);
//
//		void setNoteOnFunction(int paramNumber, int scaleLow, int scaleHi);
//
//		void setNoteOnFunction(int paramNumber, float scaleLow, float scaleHi);

		virtual ~MIDIProcessor();
};

}

#endif /* SRC_COREMIDIDEVICES_MIDIPROCESSOR_H_ */
