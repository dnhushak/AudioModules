#pragma once
#include "IAudio.hpp"
#include "chiputil.hpp"
#include <vector>
#include <iostream>

namespace chip {
	class Mixer: public IAudio {
		public:
			// Constructor
			Mixer(int);

			// Advance by a given number of samples (in this case summing all in the AudioList
			float * advance(int);

			// Adds audio object to mixer list
			void addObjects(IAudio *);

			// Removes audio object from mixer list based on audio object reference
			void removeObjects(IAudio*);

			// Removes audio object from mixer based on vector index
			void removeObjects(int);

			// Resizes the mixer buffer size
			void resizeBuffer(int);

			// Set every value in the buffer to 0
			void zeroBuffer();

			// Remove all objects from the mixer
			void clearMixer();

			// Returns the number of objects in this mixer
			int getNumObjects();

		private:

			// Audio buffers
			float * mixed;
			float * temp;
			int bufferSize;

			// List of Audio items to mix together
			std::vector<IAudio*> * audioList;
	};
}
