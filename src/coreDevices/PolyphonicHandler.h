#ifndef POLYPHONICHANDLER_H_
#define POLYPHONICHANDLER_H_

#include "Connectable.h"
#include "Device.h"
#include <map>

namespace device {
	
	class PolyphonicHandler: public device::Device,
			public device::Connectable<Device, Device> {
		public:
			PolyphonicHandler();

			/**
			 * Sets the maximum number of voices the handler will allow. Generally set lower for processor-intensive voices,
			 * or on machines with poor performance. -1 means infinite number of voices allowed.
			 * @param newMaxNumVoices
			 */
			void setMaxNumVoices(int newMaxNumVoices);

			/**
			 * Sets the number of timbres for multitimbral instruments. Minimum is 1.
			 * @param newNumTimbres
			 */
			void setNumTimbres(int newNumTimbres);

			/**
			 * Sets the device that all voices will attach to upstream. This allows the ```PolyphonicHandler``` to remain
			 * ignorant of its attached devices' classes. For example, a mixer could be the upstream device, which each
			 * voice activation creating a new audio device(s), which then attach to the mixer. The handler literally only
			 * handles the creation, connection, and deletion of voices, and not the actual audio or I/O actions of said devices.
			 * @param upstream
			 */
			void setUpstream(Connectable<Device, Device> * newUpstream);

			/**
			 * Disconnects the voices from the upstream device, and prevents any further created voices from being connected
			 * to a device.
			 *
			 * Note that this does NOT delete upstream, nor does it remove any non-polyphonic device from its list, it only
			 * removes the devices that this particular handler is handling
			 */
			void disconnectUpstream();

			/**
			 * Activates or updates a voice, based on a voice number (commonly a MIDI note number)
			 * @param voiceNumber
			 */
			void activateVoice(int voiceNumber);

			/**
			 * Deactivates a voice, based on a voice number (commonly a MIDI note number)
			 * @param voiceNumber
			 */
			void deactivateVoice(int voiceNumber);

			/**
			 * Runs through each item in the upstream's list, and if the ```state``` is ```inactive```, remove it from the upstream list
			 */
			void cleanup();

			virtual ~PolyphonicHandler();

		private:
			int maxNumVoices;
			int numTimbres;
			int curTimbre;
			int numVoices;
			std::map<int, Device *> voiceMap;
			std::map<int, Device *>::iterator voiceIter;
			Connectable<Device, Device> * upstream;
	};

}

#endif /* POLYPHONICHANDLER_H_ */
