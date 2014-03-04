#include "MockIAudio.hpp"

chip::MockIAudio::MockIAudio(int bufferSize) {
	buffer = new float[bufferSize];
}

float * chip::MockIAudio::advance(int num) {
	return buffer;
}

