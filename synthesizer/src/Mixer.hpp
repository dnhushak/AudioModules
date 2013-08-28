#pragma once
#include "IAudio.hpp"
#include <vector>

namespace chip
{
	class Mixer: public IAudio
	{
		public:
			std::vector<IAudio*> audioList; // each of the IAudio objects that will be added together
			Mixer(); //empty constructor
			Mixer(std::vector<IAudio*>); //constructor takes list of IAudio objects 
			std::vector<float> advance(int); //the 0th elements are all added together, the 1st elements, 2nd, all the way to the 
										//nth elements and the result is returned -- aka move along the sound wave
			void addObjects(IAudio*); //used in construction, adds an IAudio object to audioList
	};
}	
