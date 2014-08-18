#ifndef WAVETABLE_HPP_
#define WAVETABLE_HPP_

#include "AudioUtils.hpp"
#include "Device.hpp"
// For memset, memcpy, etc..
#include <string.h>

namespace audio {
	
	class Wavetable : public device::Device {
		public:
			// Constructor. Takes a table size as input
			Wavetable(int);

			// Return the sample value at a given location in the table
			sample_t getSample(int);

			// Set a sample to a value at a location
			void setSample(int, sample_t);

			// Return the whole table
			sample_t * getTable();

			// Return the table size
			// Note, no setting of table size, very bad to do this. Remake a new wavetable if using a different size
			int getTableSize();

			// Insert a new table
			void populateTable(sample_t *, int);

			~Wavetable();

		protected:
			int tableSize;

			sample_t * table;
	};

}

#endif //WAVETABLE_HPP_
