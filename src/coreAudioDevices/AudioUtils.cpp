#include "AudioUtils.hpp"

float ratioToDb(float ratio) {
	return 20 * log10f(ratio);
}

float dbToRatio(float db) {
	// Volume is coming in in dbfs
	// 0.0 dbfs = gain of 1
	// gain (db) = 20*log10(in/out)
	// Therefore, in/out = 10^(gain(db)/20)
	return pow(10, (db / 20));
}

//Midi Note to Frequency
float MtoF(int note) {
	return (float) pow(2, (((double) note - 69) / 12.0)) * 440.0;
}

int sampToMs(int samples, int sampleRate) {
	return (1000 * samples) / sampleRate;
}

int msToSamp(int ms, int sampleRate) {
	return (ms * sampleRate) / 1000;
}
