/*
 * Filter.cpp
 *
 *  Created on: Mar 6, 2014
 *      Author: dnhushak
 */

#include "Filter.hpp"

Filter::Filter() {
	
}

float * Filter::advance(int numSamples) {
	if (getNumObjects() > 0) {
		return (*audioDeviceList)[0]->advance(numSamples);
	} else {
		zeroBuffer();
		return buffer;
	}
}

