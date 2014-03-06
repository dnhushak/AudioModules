#pragma once
#include "Wavetables.hpp"
#include "chiputil.hpp"
#include <vector>
#include <iostream>

namespace chip {
	class PolyVoice: public AudioClass {
		public:
			PolyVoice(int);
			~PolyVoice() {
			}

			unsigned int stepsize();

			// Sets the state of the polyvoice to release
			void releasePolyVoice();

			// Sets the ADSR parameters for this polyvoice
			void setVoice(int, int, float, int, int, float, int, int);

			float * advance(int);
			void zeroBuffer();

		private:

			float * buffer;
			int bufferSize;

			int state;

			int note;
			unsigned short phase;
			float frequency;

			Wavetables* wavetable;

			// ADSR envelope parameters
			bool envelope;
			int waveType;

			float vibAmp;
			int vibPeriod;
			int vibDelay;
			int vibCount;
			float vibFreq;

			virtual ~PolyVoice();
	};

}
