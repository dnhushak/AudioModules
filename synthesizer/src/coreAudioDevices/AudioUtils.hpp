#ifndef AUDIOUTILS_HPP_
#define AUDIOUTILS_HPP_

#include <math.h>

namespace synth {
	// Sample type, min, and max
	typedef float sample_t;
	static const sample_t sampleMax = 1.0;
	static const sample_t sampleMin = -1.0;

}
// Converts a straight I/O ratio to a db gain
float ratioToDb(float ratio);

// Converts a db gain value into a straight I/O ratio
float dbToRatio(float db);

//Midi Note number to Frequency
float MtoF(int note);

#endif //AUDIOUTILS_HPP_
