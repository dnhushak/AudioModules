#ifndef POLYPHONICHANDLER_H_
#define POLYPHONICHANDLER_H_

#include "Connectable.h"
#include "Device.h"
#include <map>

namespace device {

	/**
	 * ```PolyphonicHandler``` is a class used to do just that: handle polyphony.
	 *
	 * Essentially instead of creating all of the voices one could want at runtime, PH will dynamically create (and destroy) voices as needed
	 *
	 * It works much like a modular synthesizer, with the added bonus of digital duplicity
	 *
	 * Since PH is a ```Connectable``` object, you can connect a Device (or a string of ```Connectable``` Devices) to it, and it will automatically duplicate
	 * the entire tree when a new voice is needed. It will then attach the head of that new tree to the upstream device.
	 *
	 * Note that the template class declarations *have to match* the class declarations within ```Connectable``` of the upstream class type
	 *
	 * Note also that Upstream HAS to be ```Connectable```
	 *
	 * I.E. a common example is a ```Mixer``` as the upstream object. ```Mixer``` is of type ```Connectable<AudioDevice,AudioDevice>```, so declaring a
	 * PolyphonicHandler that will have a mixer as its upstream device, you would declare it as ```PolyphonicHandler<AudioDevice,AudioDevice>```
	 *
	 * PH only allows for one object to connect to it, and it will treat this as the "head" of the tree to recursively duplicate for every voice
	 */
	template<class UpstreamInheritedType, class UpstreamConnectingType>
	class PolyphonicHandler: public Connectable<Device, UpstreamConnectingType> {
			typedef Connectable<UpstreamInheritedType, UpstreamConnectingType> UpstreamType;
		public:
			PolyphonicHandler(){
				numVoices = 0;
				maxNumVoices = -1;
				upstream = 0;

				// Can only have 1 device attached to it,
				// that device and any subsequently connected
				// devices will be the base "voice" for any polyphony
				this->setMaxNumDevices(1);
			}

			/**
			 * Sets the maximum number of voices the handler will allow. Generally set lower for processor-intensive voices,
			 * or on machines with poor performance. -1 means infinite number of voices allowed.
			 * @param newMaxNumVoices
			 */
			void setMaxNumVoices(int newMaxNumVoices){
				if (newMaxNumVoices == -1 || newMaxNumVoices > 0){
					maxNumVoices = newMaxNumVoices;
				}
			}

			/**
			 * Sets the device that all voices will attach to upstream. This allows the ```PolyphonicHandler``` to remain
			 * ignorant of its attached devices' classes. For example, a mixer could be the upstream device, which each
			 * voice activation creating a new audio device(s), which then attach to the mixer. The handler literally only
			 * handles the creation, connection, and deletion of voices, and not the actual audio or I/O actions of said devices.
			 * @param upstream
			 */
			void setUpstream(UpstreamType * newUpstream){
				upstream = newUpstream;
				// Start at the beginning of the voice map
				voiceIter = voiceMap.begin();
				// Iterate over the whole map
				for (voiceIter = voiceMap.begin(); voiceIter != voiceMap.end();
						voiceIter++){
					// Add all of the devices in the voice map from the upstream device
					upstream->connectDevice(voiceIter->second);
				}
			}

			UpstreamType * getUpstream(){
				return upstream;
			}

			/**
			 * Disconnects the voices from the upstream device, and prevents any further created voices from being connected
			 * to a device.
			 *
			 * Note that this does NOT delete upstream, nor does it remove any non-polyphonic device from its list, it only
			 * removes the devices that this particular handler is handling
			 */
			void disconnectUpstream(){
				// Check if upstream still exists
				if (upstream != 0){
					// Iterate over the whole map
					for (voiceIter = voiceMap.begin();
							voiceIter != voiceMap.end(); voiceIter++){
						// Get rid of all of the devices in the voice map from the upstream device
						upstream->disconnectDevice(voiceIter->second);
					}
				}
				upstream = 0;
			}

			/**
			 * Activates or updates a voice, based on a voice number (commonly a MIDI note number)
			 * @param voiceNumber
			 * @param param The parameter to pass to a new voice, typically velocity from a MIDI controller
			 */
			void activateVoice(int voiceNumber, Parameter param){
				// Check if voiceNumber already exists
				// If it does, update it with new parameter
				if (voiceMap.count(voiceNumber)){
					voiceMap[voiceNumber]->setState(ACTIVE);
					voiceMap[voiceNumber]->alter(0, param);
				}

				// If it doesn't...
				else{
					// Check to see if we have voices available
					if (maxNumVoices == -1 || numVoices < maxNumVoices){
						// Increment the voice counter
						numVoices++;

						if (!this->isEmpty()){
							// Copy the current device tree
							UpstreamConnectingType * newTree =
									this->front()->clone(this->WHOLETREE);

							newTree->alter(0, param);

							// Add the new tree to the voiceMap
							voiceMap.insert(
									std::pair<int, UpstreamConnectingType *>(
											voiceNumber, newTree));

							// If there is an upstream device, attach the new device tree to upstream
							if (upstream != 0){
								upstream->connectDevice(newTree);
							}
						}
					}
				}
			}

			/**
			 * Deactivates a voice, based on a voice number (commonly a MIDI note number)
			 * @param voiceNumber
			 */
			void deactivateVoice(int voiceNumber){
				// Check if voiceNumber does exist in map
				if (voiceMap.count(voiceNumber)){

					//TODO: Figure out how to best work with envelopes and other time-based things that have alternative states - utilize Alter fcn??
					// Deactivate it
					voiceMap[voiceNumber]->setState(INACTIVE);

					// Decrement the voice counter
					numVoices--;
				}
			}

			/**
			 * Runs through each item in the upstream's list, and if the ```state``` is ```inactive```, remove it from the upstream list
			 */
			void cleanup(){
				voiceIter = voiceMap.begin();
				// Iterate over the whole map
				for (voiceIter = voiceMap.begin(); voiceIter != voiceMap.end();
						voiceIter++){
					// Check for inactive voices
					if (voiceIter->second->getState() == INACTIVE){
						if (upstream != 0){
							// Disconnect the device from upstream
							upstream->disconnectDevice(voiceIter->second);
						}
						voiceIter->second->erase(this->SAMETREE);
						voiceMap.erase(voiceIter);
						voiceIter = voiceMap.begin();
					}
				}
			}

			virtual ~PolyphonicHandler(){
				voiceIter = voiceMap.begin();
				for (voiceIter = voiceMap.begin(); voiceIter != voiceMap.end();
						voiceIter++){
					voiceIter->second->setState(INACTIVE);
				}
				cleanup();
			}

		private:
			int maxNumVoices;
			int numVoices;
			std::map<int, UpstreamConnectingType *> voiceMap;
			typename std::map<int, UpstreamConnectingType *>::iterator voiceIter;

			UpstreamType * upstream;

	};
}

#endif /* POLYPHONICHANDLER_H_ */
