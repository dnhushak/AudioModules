#ifndef CONNECTABLEDEVICE_h_
#define CONNECTABLEDEVICE_h_
#include <cstdlib>
// For lists
#include <list>
// For iterators
#include <algorithm>

namespace device {
	template<class ConnectType, class Self>
	class ConnectableDevice {
		public:
			ConnectableDevice() {
				maxNumDevices = -1;
			}

			/**
			 * Handles duplication/simultaneous connection of children, called in devices' ```duplicate()``` function
			 *
			 * @param newDevice The new device that has been duplicated - the one to attach the children to
			 * @param children Determines behavior: 1 - recursively duplicate children, and then attach to ```newDevice```;
			 * 2 - simply attach all children of ```this``` to ```newDevice```
			 */
			Self * cloneWithConnected() {
				// Clone self
				Self * newDevice = this->clone();

				if (!isEmpty()) {
					// Start at the beginning of the device list
					deviceIter = begin();
					while (deviceIter != end()) {
						// Recursively call cloneWithConnected on each connected device
						ConnectType * newChild =
								(ConnectType *) ((ConnectableDevice * )(*deviceIter))->cloneWithConnected();
						// Connect newly cloned child device to the parent clone
						newDevice->connectDevice(newChild);
						// Increment the iterator
						deviceIter++;
					}
				}
				return newDevice;
			}

			Self * cloneAndConnect() {
				// Clone self
				Self * newDevice = this->clone();
				if (!isEmpty()) {
					// Start at the beginning of the device list
					deviceIter = begin();
					while (deviceIter != end()) {
						// Select each connected device
						ConnectType * newChild = (*deviceIter);
						// Connect child device to the parent clone
						newDevice->connectDevice(newChild);
						// Increment the iterator
						deviceIter++;
					}
				}
				return newDevice;
			}

			Self * clone() {
				Self * newDevice = new Self();
				return newDevice;
			}

			typename std::list<ConnectType *>::iterator begin() {
				return deviceList.begin();
			}

			typename std::list<ConnectType *>::iterator end() {
				return deviceList.end();
			}

			ConnectType * front() {
				return deviceList.front();
			}

			ConnectType * back() {
				return deviceList.back();
			}

			ConnectType * getDeviceByLoc(int loc) {
				// If loc is a valid number (device exists)
				if (loc >= 0 && loc < getNumDevices()) {
					// Start at the beginning
					typename std::list<ConnectType *>::iterator iter = begin();
					// Advance to the location
					for (int i = 0; i < loc; i++) {
						iter++;
					}
					// Return a pointer to the device
					return *iter;
				} else if (loc >= getNumDevices()) {
					// If requested location is too large, return the last device
					return back();
				} else {
					// If it's too small, return the first device
					return front();
				}

			}

			// Adding Devices
			void connectDevice(ConnectType * newDevice) {
				// If there is space left (maximum checks)
				if (hasSpace()) {
					// Add the device to the end of the list
					deviceList.push_back(newDevice);
				}
			}

			void connectDevice(int loc, ConnectType * newDevice) {
				// If there is space left
				if (hasSpace() && loc < getNumDevices()) {
					// Start at the beginning, and advance to the location indicated
					deviceIter = deviceList.begin();
					for (int i = 0; i < loc; i++) {
						deviceIter++;
					}
					// Add the device at the location
					deviceList.insert(deviceIter, newDevice);
				}
			}

			void connectDevices(std::list<ConnectType *> * newDeviceList) {
				// Start at the beginning of the new device list
				deviceIter = newDeviceList->begin();
				// Make sure we have space in our device list, and that there are still devices in the new one
				while (hasSpace() && deviceIter != newDeviceList->end()) {
					// Add the device and increment the iterator
					connectDevice((*deviceIter));
					deviceIter++;
				}
			}

			// Replacing Devices
			void replaceDevice(int loc, ConnectType * newDevice) {
				// If there is space left
				if (loc < getNumDevices()) {
					// Start at the beginning, and advance to the location indicated
					deviceIter = deviceList.begin();
					for (int i = 0; i < loc; i++) {
						deviceIter++;
					}
					// Replace the device at the location
					(*deviceIter) = newDevice;
				}

			}

			void replaceDevice(ConnectType * oldDevice,
					ConnectType * newDevice) {
				std::replace(deviceList.begin(), deviceList.end(), oldDevice,
						newDevice);
			}

			// Removing Devices
			void disconnectDevice(ConnectType * disconnectDevice) {
				// Remove device by equality
				deviceList.remove(disconnectDevice);
			}

			void disconnectDevice(int loc) {
				if (loc < getNumDevices()) {
					// Remove device by location
					deviceIter = deviceList.begin();
					for (int i = 0; i < loc; i++) {
						deviceIter++;
					}
					deviceList.erase(deviceIter);
				}
			}

			void disconnectAllDevices() {
				// Remove all Devices
				deviceList.clear();
			}

			// Get and set info
			int hasSpace() {
				// Returns true (1) if number of devices is less than the max, or there is no max (-1)
				return (getNumDevices() < maxNumDevices || maxNumDevices == -1);
			}

			bool isEmpty() {
				return (getNumDevices() == 0);
			}

			int getNumDevices() {
				return deviceList.size();
			}

			int getMaxNumDevices() {
				return maxNumDevices;
			}

			~ConnectableDevice() {
			}

		protected:
			void setMaxNumDevices(int newMax) {
				// Check for valid input
				if (newMax < -1) {
					return;
				}
				// Shrink list if max is now smaller
				if (newMax < maxNumDevices && newMax != -1) {
					deviceList.resize(newMax);
				}
				// Set the maxnum to the new maximum
				maxNumDevices = newMax;
			}

			// List info
			typename std::list<ConnectType *> deviceList;
			typename std::list<ConnectType *>::iterator deviceIter;
			int maxNumDevices;
	}
	;

}

#endif /* CONNECTABLEDEVICE_h_ */
