//Audio Object Interface
#pragma once

#include <vector>

namespace chip {
	class IAudio {
			//no member variables
		public:
			//Will be different for each IAudio implementation
			virtual float * advance(int) = 0;

			//virtual ~IAudio() = 0;
	};
}

