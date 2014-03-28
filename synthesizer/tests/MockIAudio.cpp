#include "MockIAudio.hpp"

synth::MockIAudio::MockIAudio(int bufferSize) {
	buffer = new float[bufferSize];
}

float * synth::MockIAudio::advance(int num) {
	return buffer;
}

