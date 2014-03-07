/*
 * Wavetable.cpp
 *
 *  Created on: Mar 6, 2014
 *      Author: dnhushak
 */

#include "Wavetable.hpp"

namespace chip {
	
	Wavetable::Wavetable(int initTableSize) {
		tableSize = initTableSize;
		table = new float[tableSize];
	}

	// Get a sample value at a specified index
	float Wavetable::getSample(int index) {
		if (index < tableSize) {
			return table[index];
		} else {
			return 0;
		}
	}

	// Set a sample to a value at a specified index
	void Wavetable::setSample(int index, float newVal) {
		if (index < tableSize) {
			table[index] = newVal;
		}
	}

	// Get the whole table
	float * Wavetable::getTable() {
		return table;
	}

	// Get the wavetable size
	int Wavetable::getTableSize() {
		return tableSize;
	}

	// Set the table to a new table
	void Wavetable::populateTable(float * newTable) {
		table = newTable;
	}

}
