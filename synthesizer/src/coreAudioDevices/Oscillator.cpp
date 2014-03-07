#include "Oscillator.hpp"

chip::Oscillator::Oscillator(int initBufferSize, int initSampleRate) {
	bufferSize = initBufferSize;
	sampleRate = initSampleRate;
	phase = 0;
	frequency = 0;
	wavetable = NULL;
}

float * chip::Oscillator::advance(int numSamples) {
	for (int i = 0; i < numSamples; i++) {
		buffer[i] = 0;;
		//phase += stepsize();
	}
	return buffer;
}

unsigned int chip::Oscillator::stepsize() {
//Maximum value of phase scale (16^4 in this case)
	int step;

//Our equation!
	step = ((frequency) * PHASESCALE) / sampleRate;
	return step;
}
