#ifndef CONNECTABLEDEVICE_h_
#define CONNECTABLEDEVICE_h_
#include <cstdlib>
// For lists
#include <list>
// For iterators
#include <algorithm>

namespace device {
	template<class A>
	class ConnectableDevice: public Device {
		public:

			ConnectableDevice() {
				maxNumDevices = -1;
			}

			typename std::list<A *>::iterator begin() {
				return deviceList.begin();
			}

			typename std::list<A *>::iterator end() {
				return deviceList.end();
			}

			A * front() {
				return deviceList.front();
			}

			A * back() {
				return deviceList.back();
			}

			A * getDeviceByLoc(int loc) {
				// If loc is a valid number (device exists)
				if (loc >= 0 && loc < getNumDevices()) {
					// Start at the beginning
					typename std::list<A *>::iterator iter = begin();
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
			void addDevice(A * newDevice) {
				// If there is space left (maximum checks)
				if (hasSpace()) {
					// Add the device to the end of the list
					deviceList.push_back(newDevice);
				}
			}

			void addDevice(int loc, A * newDevice) {
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

			void addDevices(std::list<A *> * newDeviceList) {
				// Start at the beginning of the new device list
				deviceIter = newDeviceList->begin();
				// Make sure we have space in our device list, and that there are still devices in the new one
				while (hasSpace() && deviceIter != newDeviceList->end()) {
					// Add the device and increment the iterator
					addDevice((*deviceIter));
					deviceIter++;
				}
			}

			// Replacing Devices
			void replaceDevice(int loc, A * newDevice) {
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
			void replaceDevice(A * oldDevice, A * newDevice) {
				std::replace(deviceList.begin(), deviceList.end(), oldDevice,
						newDevice);
			}

			// Removing Devices
			void removeDevice(A * removeDevice) {
				// Remove device by equality
				deviceList.remove(removeDevice);
			}

			void removeDevice(int loc) {
				if (loc < getNumDevices()) {
					// Remove device by location
					deviceIter = deviceList.begin();
					for (int i = 0; i < loc; i++) {
						deviceIter++;
					}
					deviceList.erase(deviceIter);
				}
			}

			void removeAllDevices() {
				// Remove all Devices
				deviceList.clear();
			}

			// Get and set info
			int hasSpace() {
				// Returns true (1) if number of devices is less than the max, or there is no max (-1)
				return (getNumDevices() < maxNumDevices || maxNumDevices == -1);
			}

			int isEmpty() {
				return (getNumDevices() == 0);
			}

			int getNumDevices() {
				return deviceList.size();
			}

			int getMaxNumDevices() {
				return maxNumDevices;
			}

			/**
			 * Recursively searches for and returns a list of pointers to all devices connected to this one
			 * @return
			 */
			std::list<Device *> * getAllConnectedDevices() {
//				// Create list
				std::list<Device *> connectedDeviceList =
						new std::list<Device *>();
//				// Add self to list
//				connectedDeviceList.add(this);
//
//				// Start with the first device
//				deviceIter = deviceList->begin();
//				while (deviceIter != deviceList->end()) {
//					// Try a recursive call to getAllConnectedDevices(), indicating it is a connectable device
//
////						connectedDeviceList.add((*deviceIter)->getAllConnectedDevices());
//
////						connected
//
//				}
				return connectedDeviceList;
//
			}
//			/**
//			 * Recursively copies every connected device and returns the head of the tree
//			 * @return A pointer to the head of the newly copied tree
//			 */
//			A * copyConnectedTree() {
//				A * head = this.clone();
//
//
//			}

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
			typename std::list<A *> deviceList;
			typename std::list<A *>::iterator deviceIter;
			int maxNumDevices;
	}
	;

}

#endif /* CONNECTABLEDEVICE_h_ */
