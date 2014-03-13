#ifndef LIMITER_HPP_
#define LIMITER_HPP_
#include "AudioEffect.hpp"
#include "AudioUtils.hpp"

namespace chip {
	
	class Limiter: public chip::AudioEffect {
		public:
			Limiter(int, int);
			float * advance(int);
			void setThreshold(float);
		private:
			float thresholdHi;
			float thresholdLo;
	};

}

#endif /* LIMITER_HPP_ */
