#ifndef AUDIOUTILS_HPP_
#define AUDIOUTILS_HPP_

#include <math.h>
#include <limits.h>

namespace audio {
	//	// Sample type, min, and max
	typedef int sample_t;
	static const sample_t sampleMax = INT_MAX / 32;
	static const sample_t sampleMin = INT_MIN / 32;
	//	typedef float sample_t;
	//	static const sample_t sampleMax = 1.0;
	//	static const sample_t sampleMin = -1.0;

	// Converts a straight I/O ratio to a db gain
	float ratioToDb(float ratio);

	// Converts a db gain value into a straight I/O ratio
	float dbToRatio(float db);

	//Midi Note number to Frequency
	float MtoF(int note);

	/**
	 * Converts a semitone value to a multiplicative ratio
	 * @param semitones Number of semitones (up or down)
	 * @return Ratio to multiply a frequency by to raise or
	 * lower by input number of semitones
	 */
	float semitonesToRatio(float semitones);

	/**
	 * Converts a number of samples to milliseconds
	 * @param samples Number of samples
	 * @param sampleRate The desired sample rate, in samples per second
	 * @return Time the input samples represents, in milliseconds
	 */
	int sampToMs(int samples, int sampleRate);

	/**
	 * Converts a time in milliseconds to a number of samples
	 * @param ms Time in milliseconds
	 * @param sampleRate Desired sample rate, in samples per second
	 * @return Number of samples that the given time represents
	 */
	int msToSamp(int ms, int sampleRate);
}
#endif //AUDIOUTILS_HPP_
