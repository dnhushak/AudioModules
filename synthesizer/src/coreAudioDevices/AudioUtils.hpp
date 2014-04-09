#ifndef AUDIOUTILS_HPP_
#define AUDIOUTILS_HPP_

#include <math.h>
#include <limits.h>

namespace synth {
	// Sample type, min, and max
	typedef int sample_t;
	static const sample_t sampleMax = INT_MAX;
	static const sample_t sampleMin = INT_MIN;

	// Size of buffer
	static int bufferSize = 64;

	// Sampling rate of the device
	static int sampleRate = 44100;

}

// Converts a straight I/O ratio to a db gain
float ratioToDb(float ratio);

// Converts a db gain value into a straight I/O ratio
float dbToRatio(float db);

//Midi Note number to Frequency
float MtoF(int note);

#endif //AUDIOUTILS_HPP_
