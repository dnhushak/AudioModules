#include "Wavetable.hpp"

namespace audio {
	
	Wavetable::Wavetable(int initTableSize) {
		tableSize = initTableSize;
		table = (sample_t*) malloc(sizeof(sample_t) * tableSize);
	}

	// Get a sample value at a specified index
	sample_t Wavetable::getSample(int index) {
		if (index < tableSize) {
			return table[index];
		} else {
			return 0;
		}
	}

	// Set a sample to a value at a specified index
	void Wavetable::setSample(int index, sample_t newVal) {
		if (index < tableSize) {
			table[index] = newVal;
		}
	}

	// Get the whole table
	sample_t * Wavetable::getTable() {
		return table;
	}

	void Wavetable::setTableSize(int newSize) {
		if (newSize > 0) {
			tableSize = newSize;
			table = (sample_t *) realloc(table, sizeof(sample_t) * tableSize);
		}
	}
	// Get the wavetable size
	int Wavetable::getTableSize() {
		return tableSize;
	}

	// Copies a table from another table
	void Wavetable::populateTable(sample_t * newTable, int newSize) {
		if (newSize != tableSize) {
			table = (sample_t*) realloc(table, sizeof(sample_t) * newSize);
			tableSize = newSize;
		}
		memcpy(table, newTable, tableSize);
	}

	Wavetable::~Wavetable() {
		free(table);
	}

}
