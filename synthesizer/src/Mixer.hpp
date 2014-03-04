#pragma once
#include "IAudio.hpp"
#include "chiputil.hpp"
#include <vector>
#include <iostream>

namespace chip {
class Mixer: public IAudio {
public:
	// Audio buffers
	std::vector<float> * mixedFinal;
	std::vector<float> * temp;

	// List of Audio items to mix together
	std::vector<IAudio*> * audioList; // each of the IAudio objects that will be added together

	// Constructor
	Mixer();

	// Advance by a certian number of samples (in this case summing all in the AudioList
	std::vector<float> * advance(int);

	// Adds audio object to mixer list
	void addObjects(IAudio *);

	// Removes audio object from mixer list based on audio object reference
	void removeObjects(IAudio*);

	// Removes audio object from mixer based on vector index
	void removeObjects(int);

	// Destructor
	virtual ~Mixer();
};
}
