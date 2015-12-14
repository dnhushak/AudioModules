#ifndef WAVETABLE_h_
#define WAVETABLE_h_

#include "AudioUtils.h"
#include "Device.h"
// For memset, memcpy, etc..
#include <string.h>

namespace audio {
	
	class Wavetable : public device::Device {
		public:
			// Constructor. Takes a table size as input
			Wavetable(int);

			virtual Wavetable * clone(int);

			// Return the sample value at a given location in the table
			sample_t getSample(int);

			// Set a sample to a value at a location
			void setSample(int, sample_t);

			void setTableSize(int);

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

#endif //WAVETABLE_h_
