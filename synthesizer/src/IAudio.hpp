//Audio Object Interface
#pragma once

#include <vector>

namespace chip
{
	class IAudio
	{
        //no member variables
		public:
		
//		    virtual ~IAudio() = 0;
			
			//Will be different for each IAudio implementation
			virtual std::vector<float> advance(int) = 0;
	};
}

