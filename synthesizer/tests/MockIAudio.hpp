#include "../src/IAudio.hpp"
#include <vector>

namespace chip {
	class MockIAudio: public IAudio {
		public:

			MockIAudio(int);
			float * advance(int num);

			float * buffer;

	};
}
