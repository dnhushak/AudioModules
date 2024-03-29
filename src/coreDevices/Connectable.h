#ifndef CONNECTABLE_h_
#define CONNECTABLE_h_
#include <cstdlib>
// For lists
#include <list>
// For iterators
#include <algorithm>

namespace device {

	/**
	 * This is the template file for a ```Connectable``` Class.
	 * The declaration is ```class Object: public Connectable<InheritType, ConnectType>```,
	 * where ```InheritType``` is the class that the Object inherits from,
	 * and ```ConnectType``` is the class of objects that can connect to this Object.
	 *
	 * Do note that ```InheritType``` MUST be of type ```Device``` or child thereof
	 */
	template<class InheritType, class ConnectType>
	class Connectable: public InheritType{
		public:
			Connectable(){
				maxNumDevices = -1;
			}

			virtual Connectable<InheritType, ConnectType> * clone(
					int cloneType){
				Connectable<InheritType, ConnectType> * newDevice = this->clone(
						0);
				switch (cloneType){
					case SELF:
						break;
					case SAMETREE:
						if (!isEmpty()){
							// Start at the beginning of the device list
							deviceIter = begin();
							while (deviceIter != end()){
								newDevice->connectDevice(*deviceIter);
								deviceIter++;
							}
						}
						break;
					case WHOLETREE:
						if (!isEmpty()){
							// Start at the beginning of the device list
							for (deviceIter = begin(); deviceIter != end();
									deviceIter++){
								// Recursively call clone on each connected device
								ConnectType * newChild =
										(ConnectType *) (*deviceIter)->clone(
												WHOLETREE);
								// Connect newly cloned child device to the parent clone
								newDevice->connectDevice(newChild);
							}
						}
						break;
				}
				return newDevice;
			}

			typename std::list<ConnectType *>::iterator begin(){
				return deviceList.begin();
			}

			typename std::list<ConnectType *>::iterator end(){
				return deviceList.end();
			}

			ConnectType * front(){
				return deviceList.front();
			}

			ConnectType * back(){
				return deviceList.back();
			}

			ConnectType * getDeviceByLoc(int loc){
				// If loc is a valid number (device exists)
				if (loc >= 0 && loc < getNumDevices()){
					// Start at the beginning
					typename std::list<ConnectType *>::iterator iter = begin();
					// Advance to the location
					for (int i = 0; i < loc; i++){
						iter++;
					}
					// Return a pointer to the device
					return *iter;
				}
				else if (loc >= getNumDevices()){
					// If requested location is too large, return the last device
					return back();
				}
				else{
					// If it's too small, return the first device
					return front();
				}

			}

			// Adding Devices
			void connectDevice(ConnectType * newDevice){
				// If there is space left (maximum checks)
				if (hasSpace()){
					// Add the device to the end of the list
					deviceList.push_back(newDevice);
				}
			}

			void connectDevice(int loc, ConnectType * newDevice){
				// If there is space left
				if (hasSpace() && loc < getNumDevices()){
					// Start at the beginning, and advance to the location indicated
					deviceIter = deviceList.begin();
					for (int i = 0; i < loc; i++){
						deviceIter++;
					}
					// Add the device at the location
					deviceList.insert(deviceIter, newDevice);
				}
			}

			void connectDevices(std::list<ConnectType *> * newDeviceList){
				// Start at the beginning of the new device list
				deviceIter = newDeviceList->begin();
				// Make sure we have space in our device list, and that there are still devices in the new one
				while (hasSpace() && deviceIter != newDeviceList->end()){
					// Add the device and increment the iterator
					connectDevice((*deviceIter));
					deviceIter++;
				}
			}

			// Replacing Devices
			void replaceDevice(int loc, ConnectType * newDevice){
				// If there is space left
				if (loc < getNumDevices()){
					// Start at the beginning, and advance to the location indicated
					deviceIter = deviceList.begin();
					for (int i = 0; i < loc; i++){
						deviceIter++;
					}
					// Replace the device at the location
					(*deviceIter) = newDevice;
				}

			}

			void replaceDevice(ConnectType * oldDevice,
					ConnectType * newDevice){
				std::replace(deviceList.begin(), deviceList.end(), oldDevice,
						newDevice);
			}

			// Removing Devices
			void disconnectDevice(ConnectType * disconnectDevice){
				// Remove device by equality
				deviceList.remove(disconnectDevice);
			}

			void disconnectDevice(int loc){
				if (loc < getNumDevices()){
					// Remove device by location
					deviceIter = deviceList.begin();
					for (int i = 0; i < loc; i++){
						deviceIter++;
					}
					deviceList.erase(deviceIter);
				}
			}

			void disconnectAllDevices(){
				// Remove all Devices
				deviceList.clear();
			}

			// Get and set info
			int hasSpace(){
				// Returns true (1) if number of devices is less than the max, or there is no max (-1)
				return (maxNumDevices == -1 || getNumDevices() < maxNumDevices);
			}

			bool isEmpty(){
				return (getNumDevices() == 0);
			}

			int getNumDevices(){
				return deviceList.size();
			}

			int getMaxNumDevices(){
				return maxNumDevices;
			}

			virtual void erase(int eraseType){
				switch (eraseType){

					case SELF:
						delete this;
						break;
					case SAMETREE:
						if (!isEmpty()){
							for (deviceIter = begin(); deviceIter != end();
									deviceIter++){
								(*deviceIter)->erase(WHOLETREE);
							}
						}
						break;
					case WHOLETREE:
						if (!isEmpty()){
							for (deviceIter = begin(); deviceIter != end();
									deviceIter++){
								(*deviceIter)->erase(WHOLETREE);
							}
						}
						delete this;
				}
				return;
			}

			virtual int getNumParameters(){
				int totalParams = Device::numParameters;
				if (!isEmpty()){
					for (deviceIter = begin(); deviceIter != end();
							deviceIter++){
						totalParams += (*deviceIter)->getNumParameters();
					}
				}
				return totalParams;
			}

			/**
			 * This is a recursive method to allow for connected device's alterable parameters to be accessible at the head of the connected tree
			 *
			 * Say we have an oscillator connected to a gain, which is then connected to a mixer:
			 *
			 * Audio Card <- Mixer <- Gain <- Oscillator
			 *
			 * A mixer has 1 parameter (activate/deactivate)
			 * A gain has 2 parameters (activate/deactivate and gain)
			 * An oscillator has 3 parameters (act/deact, set frequency (direct), and set frequency (midi number))
			 *
			 * Since they're chained together like this, one could still set the frequency of the oscillator without direct access to the
			 * oscillator's alter() function - instead, you could call mixer->alter(5, frequencyParameter) to alter the frequency of the oscillator
			 * via a midi note number
			 *
			 */
			virtual void alter(int paramNum, Parameter p){
				// If the paramNum is referencing a parameter higher than this device's parameter count, send it on to the list of devices
				if (paramNum >= Device::numParameters){
					// Decrement the parameter pointer (so downstream devices can latch onto it)
					paramNum -= Device::numParameters;

					// Start with the first device
					deviceIter = begin();

					// Increment through the attached devices until the paramNum lines up within the desired device
					while (paramNum >= (*deviceIter)->getNumParameters()){
						// paramNum is still out of range, so decrement it
						paramNum -= (*deviceIter)->getNumParameters();
						// and move on to the next device in the list
						deviceIter++;
					}
					// Forward on the parameter, with the adjusted paramNum
					(*deviceIter)->alter(paramNum, p);
				}
			}

			virtual ~Connectable(){
			}

		protected:

			void setMaxNumDevices(int newMax){
				// Check for valid input
				if (newMax < -1){
					return;
				}
				// Shrink list if max is now smaller
				if (newMax < maxNumDevices && newMax != -1){
					deviceList.resize(newMax);
				}
				// Set the maxnum to the new maximum
				maxNumDevices = newMax;
			}

			// List info
			typename std::list<ConnectType *> deviceList;
			typename std::list<ConnectType *>::iterator deviceIter;
			int maxNumDevices;

			// Types of clones to be made
			// SELF - clones just the device, its state, and all member variables
			// SAMETREE - clones just like SELF, and then connects all of the devices connected to the original to the new one (same tree, duplicate head)
			// CLONETREE - recursively clones EVERY device in the tree of devices connected to the original device (new, duplicate tree)
			enum treeAlterTypes{
				SELF, SAMETREE, WHOLETREE
			};
	};
}

#endif /* CONNECTABLE_h_ */
