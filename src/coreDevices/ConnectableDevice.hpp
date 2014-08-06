#ifndef DEVICEMANAGER_HPP_
#define DEVICEMANAGER_HPP_
#include <cstdlib>
#include <list>
#include <algorithm>

namespace modules {
	template<class A>
	class ConnectableDevice {
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

			int isEmpty(){
				return (getNumDevices() == 0);
			}

			int getNumDevices() {
				return deviceList.size();
			}

			int getMaxNumDevices() {
				return maxNumDevices;
			}

			void setMaxNumDevices(int newMax) {
				// Check for valid input
				if (newMax < -1) {
					return;
				}
				// Shrink list if max is now smaller
				if (newMax < maxNumDevices) {
					deviceList.resize(newMax);
				}
				// Set the maxnum to the new maximum
				maxNumDevices = newMax;
			}

			~ConnectableDevice() {
			}
		protected:
			// List info
			typename std::list<A *> deviceList;
			typename std::list<A *>::iterator deviceIter;
			int maxNumDevices;
	};

}

#endif /* DEVICEMANAGER_HPP_ */
