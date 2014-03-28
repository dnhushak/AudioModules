#pragma once
#include <cstdlib>

namespace synth {
	
	class Wavetable {
		public:
			// Constructor. Takes a table size as input
			Wavetable(int);

			// Return the sample value at a given location in the table
			float getSample(int);

			// Set a sample to a value at a location
			void setSample(int, float);

			// Return the whole table
			float * getTable();

			// Return the table size
			// Note, no setting of table size, very bad to do this. Remake a new wavetable if using a different size
			int getTableSize();

			// Insert a new table
			void populateTable(float *, int);

			~Wavetable();

		protected:
			int tableSize;

			float * table;
	};

}
